using EigenNet;
using System;
using System.Collections.Generic;
using System.Data;
using System.Windows.Media;

namespace CMMDataAnalysisCommon.PointFiles
{
    public class CMMObject : ObservableBase
    {
        #region Private Fields
        private string m_name = string.Empty;
        private List<Vector3d> m_points = new List<Vector3d>();
        private Color m_color = Colors.CornflowerBlue;
        private long m_lineNumber;

        public CMMObject(string featureName, int lineNumber)
        {
            Name = featureName;
            LineNumber = lineNumber;
        }
        #endregion

        #region Public Properties

        public string Name
        {
            get { return m_name; }
            set
            {
                if (m_name == value) return;
                m_name = value;
                OnPropertyChanged();
            }
        }

        public List<Vector3d> Points => m_points;
        
        public Color Color
        {
            get { return m_color; }
            set
            {
                if (m_color == value) return;
                m_color = value;
                OnPropertyChanged();
                ColorChanged?.Invoke(this, EventArgs.Empty);
            }
        }

        internal long LineNumber
        {
            get { return m_lineNumber; }
            private set
            {
                if (m_lineNumber == value) return;
                m_lineNumber = value;
                OnPropertyChanged();
            }
        }

        #endregion

        #region Events

        public event EventHandler ColorChanged;

        #endregion

        #region Validation

        protected override void GetValidationErrors(string propertyName)
        {
            switch(propertyName)
            {
                case nameof(Color):
                    ValidateColor();
                    break;
                default:
                    return;
            }
        }

        private void ValidateColor()
        {
            ClearErrors(nameof(Color));
            if (Color.A < 0 || Color.A > 255)
                AddError(nameof(Color), "Alpha must be between 0 and 255");
            if (Color.R < 0 || Color.R > 255)
                AddError(nameof(Color), "Red must be between 0 and 255");
            if (Color.G < 0 || Color.G > 255)
                AddError(nameof(Color), "Green must be between 0 and 255");
            if (Color.B < 0 || Color.B > 255)
                AddError(nameof(Color), "Blue must be between 0 and 255");
        }

        #endregion

    }
}