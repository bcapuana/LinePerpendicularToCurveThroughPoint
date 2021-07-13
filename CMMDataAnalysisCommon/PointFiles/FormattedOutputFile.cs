using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CMMDataAnalysisCommon.PointFiles
{
    public class FormattedOutputFile : DMISPointFile
    {
        #region Constructors
        protected FormattedOutputFile(string filename) : base(filename) { }

        #endregion

        #region Methods

        public static bool Open(string filename, out FormattedOutputFile fof, out string error)
        {
            fof = null;
            error = null;
            try
            {
                FormattedOutputFile temp = new FormattedOutputFile(filename);
                List<string> fileLines = new List<string>();
                using (StreamReader sr = new StreamReader(filename))
                {
                    while (!sr.EndOfStream)
                        fileLines.Add(sr.ReadLine());
                }

                List<int> featureStarts = new List<int>();
                Parallel.For(0, fileLines.Count, i =>
                {
                    if (fileLines[i].Trim().ToUpper().StartsWith("FEATURE NAME:"))
                        lock (featureStarts)
                            featureStarts.Add(i);
                });

                featureStarts.Sort();
                Parallel.For(0, featureStarts.Count, i =>
                {
                    int start = featureStarts[i],
                        end = i < featureStarts.Count - 1 ? featureStarts[i + 1] : fileLines.Count;
                    List<string> featureLines = fileLines.GetRange(start, end - start);
                    string featureName = featureLines[0].Split(new[] { ':' })[1].Trim();
                    CMMObject o = new CMMObject(featureName, i);
                    for (int j = 1; j < featureLines.Count; j++)
                    {
                        string[] splitLine = featureLines[j].Split(new[] { ',' });
                        o.Points.Add(new EigenNet.Vector3d(Convert.ToDouble(splitLine[0]), Convert.ToDouble(splitLine[1]), Convert.ToDouble(splitLine[2])));
                    }
                    lock (temp.CMMObjects)
                        temp.CMMObjects.Add(o);
                });
                temp.CMMObjects.Sort((o1, o2) => o1.LineNumber.CompareTo(o2.LineNumber));
                fof = temp;
                return true;
            }
            catch (Exception ex)
            {
                fof = null;
                error = ex.Message;
                return false;
            }

        }

        #endregion
    }
}
