# rs_directshow
RealSense DirectShow compatible driver streams color and colorized depth as one combined image. Once installed, no additional software is needed to use RealSsense
Currently, it only supports 848 x 480 x 2 (color and colorized depth) resolution at 30fps.
If you want to change camera configuration (exposure, emitter on/off and depth qualirty etc...), please use realsense-viewer in RealSense SDK.

## Setup
### Binary install
1. If you are using x64 environment and no need to modify pre-build driver, please download this project as a zip from right up green "Clone or download" button of this page.
2. Unzip download file 
3. Move to rs_directshow-master\x64\Release folder on Windows explorer. 
4. You can find install.bat at the folder. Please run it by right clicking and choosing "Run as administrator".
5. If "DLLRegisterServer in rs_directshow.dll succeeded" dialog is appered, install is successful.
6. To check RealSense with this driver, please try AmCap.exe (Simple camera capture app example of DirectSHow) in Release folder.

### Binary uninstall
1. Move to rs_directshow-master\x64\Release folder on Windows explorer. 
2. You can find uninstall.bat at the folder. Please run it by right clicking and choosing "Run as administrator".
3. If "DLLUNRegisterServer in rs_directshow.dll succeeded" dialog is appered, uninstall is successful.

### Source build
Coming soon...