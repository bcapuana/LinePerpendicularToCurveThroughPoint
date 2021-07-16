using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SetPathVar
{
    class Program
    {
        static void Main(string[] args)
        {

            Console.WriteLine(args[0]);
            string path = Environment.GetEnvironmentVariable("PATH", EnvironmentVariableTarget.Machine);

            if (!path.Contains($";{args[0]}"))
            {
                path += $";{args[0]}";
                Environment.SetEnvironmentVariable("PATH", path, EnvironmentVariableTarget.Machine);
            }
        }
    }
}
