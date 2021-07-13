using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CMMDataAnalysisCommon.PointFiles.MLEADA_PointFile
{
    public class MLEADA_PointFile
    {

        #region Private Fields

        private List<MLEADA_Section> m_sections = new List<MLEADA_Section>();

        #endregion

        #region Public Properties

        public List<MLEADA_Section> Sections => m_sections;

        #endregion

        #region Static Methods

        public static bool Open(string fileName, out MLEADA_PointFile file, out string error)
        {
            file = null;
            error = null;
            try
            {
                List<FileLine> fileLines = new List<FileLine>();
                using (StreamReader sr = new StreamReader(fileName))
                {
                    int lineNumber = 0;
                    while (!sr.EndOfStream)
                        fileLines.Add(new FileLine(++lineNumber, sr.ReadLine()));
                    sr.Close();
                }

                List<FileLine> sectionStarts = fileLines.FindAll(l => l.Text.ToUpper().Trim().StartsWith("SECTION"));
                sectionStarts.Sort((s1, s2) => s1.LineNumber.CompareTo(s2.LineNumber));
                file = new MLEADA_PointFile();
                for (int i = 0; i < sectionStarts.Count; i++)
                {
                    int start = sectionStarts[i].LineNumber,
                        end = i < sectionStarts.Count - 1 ? sectionStarts[i + 1].LineNumber : fileLines.Last().LineNumber;

                    if(!MLEADA_Section.ParseFromText(fileLines.FindAll(l=>l.LineNumber >= start && l.LineNumber < end), out MLEADA_Section section, out error))
                    {
                        file = null;
                        return false;
                    }

                    file.Sections.Add(section);
                        
                }



                return true;
            }
            catch (Exception ex)
            {
                error = ex.Message;
                file = null;
                return false;
            }
        }

        #endregion



    }
}
