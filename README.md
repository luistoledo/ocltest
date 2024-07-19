

Visual Studio project configuration (Just for reference, because the vcxproj file is already configured):

1. Open Project Properties

2. Go to C/C++ / General, and set:
  - Additional Include Directories: `%(AdditionalIncludeDirectories);src;C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v12.5\include`

3. Go to Linker /General, and set:
  - Additional Library Directories: `C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v12.5\lib\Win32;%(AdditionalLibraryDirectories)`

3. Go to Linker /Input, and set:
  - Additional Dependencies: `C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v12.5\lib\x64\OpenCL.lib;%(AdditionalDependencies)`

