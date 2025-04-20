# sirius-gcs

## Setup

1. Run `git clone https://github.com/SiriusUdS/sirius-gcs.git`.
2. Execute the `setup.bat` script in the **setup** folder.
3. Open the **sirius-gcs.sln** solution in Visual Studio 2022.
4. Compile and run.

## Create C++ file/folder

1. Create new file/folder in File Explorer.
2. If a new folder was created:
    1. Open the **sirius-gcs/premake.lua** file in a text editor.
    2. In the `includedirs` list under `workspace "sirius-gcs"`, add the relative path to your folder.
    3. Save your changes to the **sirius-gcs/premake.lua** file.
3. Execute the **sirius-gcs/setup/premake.bat** script to regenerate the Visual Studio solution.
4. Either restart Visual Studio, or click the **Reload All** button in the window that popped-up in Visual Studio. *If no window pop-up appears, it means no changes were detected.*
5. The new files and/or folders should appear in the **Solution Explorer** window.

## Enable format on save (ClangFormat)

In Visual Studio:
1. In **Tools > Options > Text Editor > C/C++ > Code Style > Formatting > General**, check the **Enable ClangFormat support** option, and under it check the **Enable ClangFormat for all formatting scenarios** option.
2. In **Analyze > Code Cleanup > Configure Code Cleanup**, select the default profile in the left window. Select the **Format Document (C++)** fixer in the bottom window and then click the arrow pointing up button. Make sure the fixer appears in the upper window.
3. The code should now get formatted automatically when saving.
