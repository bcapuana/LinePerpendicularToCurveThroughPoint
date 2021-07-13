using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CMMDataAnalysisCommon.PointFiles
{
    public class DMISPointFile : ObservableBase, IPointFile
    {
        #region Private Fields
        protected string m_filename = string.Empty;
        protected List<CMMObject> m_CMMObjects = new List<CMMObject>();
        #endregion

        #region Public Properties

        public string Filename
        {
            get { return m_filename; }
        }

        public List<CMMObject> CMMObjects
        {
            get { return m_CMMObjects; }
        }

       

        #endregion

        #region Constructors

        protected DMISPointFile() { }
        protected DMISPointFile(string filename)
        {
            m_filename = filename;
        }


        #endregion

        #region Methods

        public static bool Open(string filename, out DMISPointFile pf, out string error)
        {
            error = null;
            pf = null;
            try
            {

                List<string> fileLines = new List<string>();
                using (StreamReader sr = new StreamReader(filename))
                {
                    while (!sr.EndOfStream)
                        fileLines.Add(sr.ReadLine());
                }

                if (!GetFeatureHeaderLines(fileLines, out List<string> headerLines, out error))
                    return false;

                List<CMMObject> objects = new List<CMMObject>();
                //foreach(string headerLine in headerLines)
                List<string> errors = new List<string>();
                Parallel.ForEach(headerLines, headerLine =>
                {

                    if (!CreateNewCMMObject(fileLines, headerLine, out CMMObject obj, out string err))
                    {
                        lock (errors)
                            errors.Add(err);
                    }
                    else
                    {
                        lock (objects)
                            objects.Add(obj);
                    }
                });

                objects.Sort((o1, o2) => o1.LineNumber.CompareTo(o2.LineNumber));
                pf = new DMISPointFile(filename);
                pf.CMMObjects.AddRange(objects);

                return true;
            }
            catch (FileNotFoundException)
            {
                error = $"Could not find \"{filename}\"";
                pf = null;
                return false;
            }
            catch (Exception ex)
            {
                error = $"Error opening DMIS Point File: {ex.Message}";
                pf = null;
                return false;
            }
        }

        private static bool CreateNewCMMObject(List<string> fileLines, string headerLine, out CMMObject obj, out string error)
        {
            error = null;
            obj = null;
            try
            {

                string featureName = headerLine.Split(new[] { '(', ')' })[1];
                CMMObject tempObj = new CMMObject(featureName, fileLines.FindIndex(l => l == headerLine));
                List<string> pointLines = (from l in fileLines where l.Contains(featureName) && !l.Contains("OUTPUT") select l).ToList();
                List<string> errors = new List<string>();
                Dictionary<int, EigenNet.Vector3d> tempPoints = new Dictionary<int, EigenNet.Vector3d>();
                Parallel.For(0, pointLines.Count, i =>
                {
                    string line = pointLines[i];
                    string[] splitLine = line.Split(new[] { ',' });
                    if (!double.TryParse(splitLine[3], out double x))
                    {
                        lock (errors)
                            errors.Add("Unable to conver x value");
                    }
                    if (!double.TryParse(splitLine[4], out double y))
                    {
                        lock (errors)
                            errors.Add("Unable to conver x value");
                    }
                    if (!double.TryParse(splitLine[5], out double z))
                    {
                        lock (errors)
                            errors.Add("Unable to conver x value");
                    }
                    lock (tempPoints)
                        tempPoints.Add(i, new EigenNet.Vector3d(x, y, z));
                });
                for (int i = 0; i < tempPoints.Count; i++)
                    tempObj.Points.Add(tempPoints[i]);
                /*foreach(string line in pointLines)
                {
                    string[] splitLine = line.Split(new[] { ',' });
                    if(!double.TryParse(splitLine[3],out double x))
                    {
                        error = "Unable to conver x value";
                        return false;
                    }
                    if (!double.TryParse(splitLine[4], out double y))
                    {
                        error = "Unable to conver x value";
                        return false;
                    }
                    if (!double.TryParse(splitLine[5], out double z))
                    {
                        error = "Unable to conver x value";
                        return false;
                    }
                    tempObj.Points.Add(new EigenNet.Vector3d(x, y, z));
                }*/
                obj = tempObj;
                return true;
            }
            catch (Exception ex)
            {
                obj = null;
                error = $"Error creating feature: {ex.Message}";
                return false;
            }
        }

        private static bool GetFeatureHeaderLines(List<string> fileLines, out List<string> headerLines, out string error)
        {
            error = null;
            try
            {
                headerLines = (from l in fileLines where l.ToUpper().StartsWith("OUTPUT") select l).ToList();
                return true;
            }
            catch (Exception ex)
            {
                headerLines = null;
                error = $"Error finding feature header lines: \r\n{ex.Message}";
                return false;
            }
        }

        #endregion

    }
}
