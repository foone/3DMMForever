# escape=`

FROM mcr.microsoft.com/windows/servercore:ltsc2019

ADD https://aka.ms/vs/17/release/vs_buildtools.exe C:\TEMP\vs_buildtools.exe

RUN C:\TEMP\vs_buildtools.exe --quiet --wait --norestart --nocache `
    --add Microsoft.VisualStudio.Workload.VCTools --includeRecommended `
    --remove Microsoft.VisualStudio.Component.Windows10SDK.10240 `
    --remove Microsoft.VisualStudio.Component.Windows10SDK.10586 `
    --remove Microsoft.VisualStudio.Component.Windows10SDK.14393 `
    --remove Microsoft.VisualStudio.Component.Windows81SDK `
 || IF "%ERRORLEVEL%"=="3010" EXIT 0s

RUN rmdir /S /Q C:\\TEMP

RUN setx /M PATH "%PATH%;C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\MSBuild\Current\Bin"
RUN setx /M PATH "%PATH%;C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin"

SHELL ["powershell"]

RUN Set-ExecutionPolicy Bypass -Scope Process -Force; [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072; iex ((New-Object System.Net.WebClient).DownloadString('https://chocolatey.org/install.ps1')) 

RUN Install-PackageProvider -Name NuGet -MinimumVersion 2.8.5.201 -Force
RUN Install-Module -Name VCVars -Force
RUN pushvc (invoke-vcvars -TargetArch x86 -HostArch AMD64)
RUN choco install ninja -y

ENTRYPOINT [ "C:\\Program Files (x86)\\Microsoft Visual Studio\\2022\\BuildTools\\Common7\\Tools\\VsDevCmd.bat", "&&", "cmd.exe" ]