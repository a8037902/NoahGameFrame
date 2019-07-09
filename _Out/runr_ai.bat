cd /d %~dp0
cd Release

set /a num=100
setlocal enabledelayedexpansion
for /l %%I in (1,1,2) do (
echo !num!
echo Starting NFAIServer...
start "NFAIServer" "NFPluginLoader.exe" "Server=AIServer" "ID=!num!" "PluginAI.xml"

set /a num+=1
choice /t 2 /d y /n >nul
)







