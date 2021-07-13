using EigenNet;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CMMDataAnalysisCommon.PointFiles
{
    public interface IPointFile
    {
        #region Public Properties

        string Filename { get; }
        List<CMMObject> CMMObjects { get; }

        #endregion

        #region Methods


        #endregion
    }
}
