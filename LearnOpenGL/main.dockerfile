# Use a Windows base image with build tools
FROM mcr.microsoft.com/windows/servercore:ltsc2022

WORKDIR C:\app

COPY . .

# Install dependencies (adjust as needed)
RUN powershell -Command "choco install -y visualstudio2022buildtools"

# Compile C++ code
RUN cl.exe /EHsc main.cpp /Fe:myapp.exe

CMD ["myapp.exe"]