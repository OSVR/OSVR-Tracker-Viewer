set DEST=install
set SRC=deps\install
xcopy %SRC%\bin\osg*-osg.dll %DEST% /Y
xcopy %SRC%\bin\osg*-osgDB.dll %DEST% /Y
xcopy %SRC%\bin\osg*-osgGA.dll %DEST% /Y
xcopy %SRC%\bin\osg*-osgText.dll %DEST% /Y
xcopy %SRC%\bin\osg*-osgUtil.dll %DEST% /Y
xcopy %SRC%\bin\osg*-osgViewer.dll %DEST% /Y
xcopy %SRC%\bin\*-OpenThreads.dll %DEST% /Y

xcopy %SRC%\bin\osgPlugins* %DEST% /Y /S

xcopy %SRC%\osvr-ver.txt %SRC%\bin\osvrClientKit.dll %SRC%\bin\osvrClient.dll %SRC%\bin\osvrTransform.dll %SRC%\bin\osvrUtil.dll %DEST% /Y

xcopy redist\*.* %DEST% /Y

set /p VER=<%SRC%\osvr-ver.txt
move install OSVR-Tracker-View-built-with-%VER%
7za a OSVR-Tracker-View-built-with-%VER%.7z OSVR-Tracker-View-built-with-%VER%\*