# LinePerpendicularToCurveThroughPoint

This program accepts 3 file from modus and create a line perpendicular to the specified section curve at the specified point.

1. The first input file is the list of nominal points for each section, both inside and outside curves.
2. The second input file is the list of acutal points for each section, both inside and outside curves.
3. The third input file is the list of the points that the lines should go through

## Building the solution
1. Download install [OpenCascade](https://dev.opencascade.org/release)
2. Download and install [Eigen](https://eigen.tuxfamily.org/index.php?title=Main_Page)
3. Modify LinePerpendicularToCurveAtPoint.bat so that the locations of the BIN and INCLUDE files match your install

Open Visual Studio by running LinePerpendicularToCurveAtPoint.bat
