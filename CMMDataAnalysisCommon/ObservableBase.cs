using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace CMMDataAnalysisCommon
{
    public abstract class ObservableBase : INotifyPropertyChanged, INotifyDataErrorInfo
    {
        #region Protected Fields

        protected Dictionary<string, List<string>> m_errorsByPropertyName = new Dictionary<string, List<string>>();

        #endregion


        #region INotifyPropertyChanged

        public void OnPropertyChanged([CallerMemberName] string propertyName = null)
        {
            GetValidationErrors(propertyName);
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }

        public event PropertyChangedEventHandler PropertyChanged;

        #endregion


        #region IDataErrorInfo


        public bool HasErrors => m_errorsByPropertyName.Any();
        public IEnumerable GetErrors(string propertyName)
        {
            return m_errorsByPropertyName.ContainsKey(propertyName) ? m_errorsByPropertyName[propertyName] : null;
        }
        public event EventHandler<DataErrorsChangedEventArgs> ErrorsChanged;

        #endregion

        #region Validation

        protected virtual void GetValidationErrors(string propertyName) { }

        protected void AddError(string propertyName, string error)
        {
            if (!m_errorsByPropertyName.ContainsKey(propertyName))
                m_errorsByPropertyName[propertyName] = new List<string>();
            if (!m_errorsByPropertyName[propertyName].Contains(error))
            {
                m_errorsByPropertyName[propertyName].Add(error);
                OnErrorsChanged(propertyName);
            }
        }

        protected void ClearErrors(string propertyName)
        {
            if (m_errorsByPropertyName.ContainsKey(propertyName))
            {
                m_errorsByPropertyName.Remove(propertyName);
                OnErrorsChanged(propertyName);
            }
        }

        private void OnErrorsChanged(string propertyName)
        {
            ErrorsChanged?.Invoke(this, new DataErrorsChangedEventArgs(propertyName));
        }

        #endregion



    }
}
