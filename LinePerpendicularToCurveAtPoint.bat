
@ECHO OFF

set VS_DIR=c:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\IDE

rem include folders
set "OCCT_INCLUDE=D:\OpenCascade\opencascade-7.4.0\inc"
set "TBB_INCLUDE=D:\OpenCascade\tbb_2017.0.100\include"
set "EIGEN_INCLUDE=D:\eigen\Eigen"
rem binary folders
set "OCCT_BIN=D:\OpenCascade\opencascade-7.4.0\win64\vc14\bin"
set "OCCT_BIN_DEBUG=D:\OpenCascade\opencascade-7.4.0\win64\vc14\bind"
SET "TBB_BIN=D:\OpenCascade\tbb_2017.0.100\bin\intel64\vc14"
SET "FFMPEG_BIN=D:\OpenCascade\ffmpeg-3.3.4-64\bin"
SET "FREEIMAGE_BIN=D:\OpenCascade\freeimage-3.17.0-vc14-64\bin"
SET "FREETYPE_BIN=D:\OpenCascade\freetype-2.5.5-vc14-64\bin"

rem library folders
set "OCCT_LIB=D:\OpenCascade\opencascade-7.4.0\win64\vc14\lib"
set "OCCT_LIB_DEBUG=D:\OpenCascade\opencascade-7.4.0\win64\vc14\libd"
SET "TBB_LIB=D:\OpenCascade\tbb_2017.0.100\lib\intel64\vc14"




SET SLN_FILE=LinePerpendicularToCurveAtPoint.sln

@ECHO ON
SET VS_DIR
SET SLN_FILE
SET VS_EXE=%VS_DIR%\DEVENV.EXE
SET VS_EXE

start "" "%VS_EXE%" "%SLN_FILE%"