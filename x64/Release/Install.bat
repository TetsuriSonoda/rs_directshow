cd /d %~dp0
copy rs_directshow.dll C:\Windows\System32\
copy realsense2.dll C:\Windows\System32\
cd C:\Windows\System32\
regsvr32 rs_directshow.dll