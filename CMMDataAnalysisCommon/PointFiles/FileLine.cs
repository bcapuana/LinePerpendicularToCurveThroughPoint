using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CMMDataAnalysisCommon.PointFiles
{
    public class FileLine
    {
        #region Private Fields

        private string m_text;
        private int m_lineNumber;

        #endregion

        #region Public Properties

        public string Text => m_text;
        public int LineNumber => m_lineNumber;

        #endregion


        #region Constructors

        public FileLine(int lineNumber, string line)
        {
            m_lineNumber = lineNumber;
            m_text = line;
        }

        #endregion


    }
}
