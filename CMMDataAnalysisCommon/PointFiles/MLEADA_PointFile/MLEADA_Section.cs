using EigenNet;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text.RegularExpressions;

namespace CMMDataAnalysisCommon.PointFiles.MLEADA_PointFile
{
    public class MLEADA_Section
    {
        #region Private Fields

        protected List<Vector3d> m_insidePoints = new List<Vector3d>(),
                               m_outsidePoints = new List<Vector3d>();
        protected string m_sectionName;



        #endregion

        #region Public Properties

        public List<Vector3d> InsidePoints => m_insidePoints;
        public List<Vector3d> OutsidePoints => m_outsidePoints;

        public string SectionName => m_sectionName;

        #endregion



        #region Constructors

        public MLEADA_Section() { }

        public MLEADA_Section(string sectionName, List<Vector3d> insidePoints, List<Vector3d> outsidePoints)
        {
            m_sectionName = sectionName;
            m_insidePoints = insidePoints;
            m_outsidePoints = outsidePoints;
        }

        #endregion

        #region methods

        public void RemovePointsTooClose(double tolerance)
        {
            RemovePointsTooClose(ref m_insidePoints, tolerance);
            RemovePointsTooClose(ref m_outsidePoints, tolerance);
        }

        void RemovePointsTooClose(ref List<Vector3d> points, double tolerance)
        {
            List<Vector3d> result = new List<Vector3d>();
            result.Add(points.First());
            for(int i =1;i<points.Count;i++)
            {
                if ((points[i] - result.Last()).VectorLength > tolerance)
                    result.Add(points[i]);
            }
            points = result;
        }

        #endregion


        #region Static Methods


        internal static bool ParseFromText(List<FileLine> fileLines, out MLEADA_Section section, out string error)
        {

            section = null;
            error = null;
            int lineNumber = fileLines[0].LineNumber;
            try
            {

                if (!GetSectionName(fileLines[0], out string sectionName, out error)) return false;

                if (!GetInsideOutsideLineNumbers(fileLines, out int[] inside, out int[] outside, out error)) return false;



                List<Vector3d> insidePoints = new List<Vector3d>(),
                               outsidePoints = new List<Vector3d>();
                if (inside != null && !GetCurvePoints(fileLines, inside, out insidePoints, out error)) return false;

                if (outside != null && !GetCurvePoints(fileLines, outside, out outsidePoints, out error)) return false;

                section = new MLEADA_Section(sectionName, insidePoints, outsidePoints);

                return true;

            }
            catch (Exception ex)
            {
                error = $"Error parsing file at line {lineNumber} {ex.Message}";
                return false;
            }
        }

        private static bool GetCurvePoints(List<FileLine> fileLines, int[] pointRange, out List<Vector3d> points, out string error)
        {
            int lineNumber = 0;
            error = null;
            try
            {
                points = new List<Vector3d>();
                List<FileLine> pointLines = fileLines.FindAll(l => l.LineNumber > pointRange[0] && l.LineNumber < pointRange[1]);
                foreach (FileLine line in pointLines)
                {
                    lineNumber = line.LineNumber;
                    string[] splitString = line.Text.Split(new[] { ' ', ',', '\t' });
                    if (splitString.Length < 3)
                    {
                        error = $"Invalid point format on line {lineNumber}";
                        pointLines = null;
                        return false;
                    }
                    double[] pointArray = new double[3];
                    for (int i = 0; i < 3; i++)
                        pointArray[i] = Convert.ToDouble(splitString[i]);

                    Vector3d point = new Vector3d(pointArray);

                    if (points.Count == 0)
                        points.Add(point);
                    else if (new Vector3d(point - points[points.Count-1]).VectorLength > double.Epsilon)
                    {
                        points.Add(point);
                    }
                }

                return true;
            }
            catch (Exception ex)
            {
                error = $"Error parsing point on line {lineNumber}: {ex.Message}";
                points = null;
                return false;
            }
        }

        internal static bool GetSectionName(FileLine fileLine, out string sectionName, out string error)
        {
            int lineNumber = fileLine.LineNumber;
            sectionName = null;
            error = null;
            try
            {
                // get the section name
                string line = fileLine.Text.ToUpper().Trim();
                if (!Regex.IsMatch(line, "^SECTION .*"))
                {
                    error = $"Section name is not in the correct format on line {lineNumber}";
                    return false;
                }

                sectionName = line.Split(' ')[1];
                return true;
            }
            catch (Exception ex)
            {
                error = $"Error parsing section name on line {lineNumber}: {ex.Message}";
                return false;
            }
        }

        private static bool GetInsideOutsideLineNumbers(List<FileLine> fileLines, out int[] inside, out int[] outside, out string error)
        {
            inside = null;
            outside = null;
            error = null;
            try
            {
                // get the start line of the inside and outside parts of the section
                FileLine insideStart = fileLines.Find(l => l.Text.Trim().ToUpper() == "INSIDE"),
                         outsideStart = fileLines.Find(l => l.Text.Trim().ToUpper() == "OUTSIDE");

                int insideEnd = 0,
                    outsideEnd = 0;

                // were both the inside and outside curves found?
                if (insideStart != null && outsideStart != null)
                {
                    // yes

                    insideEnd = outsideStart.LineNumber;
                    outsideEnd = fileLines.Last().LineNumber + 1;
                    if (insideStart.LineNumber > outsideStart.LineNumber)
                    {
                        outsideEnd = insideStart.LineNumber;
                        insideEnd = fileLines.Last().LineNumber + 1;
                    }
                }
                // was only the inside found?
                else if (insideStart != null && outsideStart == null)
                {
                    // yes
                    insideEnd = fileLines.Last().LineNumber + 1;
                    outsideEnd = -1;
                }

                // was only the outside found
                else if (insideStart == null && outsideStart != null)
                {
                    insideEnd = -1;
                    outsideEnd = fileLines.Last().LineNumber + 1;
                }

                inside = insideStart != null ? new int[] { insideStart.LineNumber, insideEnd } : null;
                outside = outsideStart != null ? new int[] { outsideStart.LineNumber, outsideEnd } : null;
                return true;
            }
            catch (Exception ex)
            {
                error = $"Error find start and end points of the inside and outside curves: {ex.Message}";
                return false;
            }
        }

        #endregion



    }
}