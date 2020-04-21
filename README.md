# future_wallet

Team Members:
  * Andrew Butler-Boudakian
  * Farukh Saidmuratov 
  * Arron Look
  * Wendi Zheng
  
## Installer
### Using the Installer for Windows to use the application.
#### Installing Future Wallet App
**Warning:** The installer unloads all the object files and other application files in the directory you specify. On uninstallation, Qt removes all contents from that directory, including any existing files from before or after installation in that directory. Make sure to install specifying a new folder to prevent any loss of data. **The installer warns you about this as well.**
1. On a Windows OS, go to the [Installer Folder](./Windows%20Installer)
2. Launch the executable installer, and follow the instructions to install the Future Wallet application. (Remember to select all components)
3. Once the installer is finished, an executable **Application.exe** can be found in the directory you specified to install to. That is the Future Wallet App.

#### Uninstalling Future Wallet App
1. In the directory you installed the Future Wallet App in, launch the **management tool.exe**.
2. Select the **remove all components** option and let the uninstaller run.

### Devlopment Instructions for Compiling the Future Wallet App Installer executable
1. Create a package directory for the installable components. For more information, see Package Directory (https://doc.qt.io/qtinstallerframework/ifw-component-description.html).
2. Create a configuration file called config.xml in the config directory. It contains information about how to build the installer binaries and online repositories. For more information about the file format and available settings, see Configuration File (https://doc.qt.io/qtinstallerframework/ifw-globalconfig.html).
3. Create a package information file called package.xml in the config\meta directory. It contains settings for deployment and the installation process. For more information, see Meta Directory (https://doc.qt.io/qtinstallerframework/ifw-component-description.html#meta-directory).
4. Create installer content and copy it to the package directory. For more information, see Data Directory (https://doc.qt.io/qtinstallerframework/ifw-component-description.html#data-directory).
5. At this point you should have the following directory structure:
   FutureWalletInstaller  --> config -> config.xml & package.xml
                         |           
                          --> packages -> FutureWallet  --> data --> content of application
                                                       |
                                                        --> meta --> license.txt & package.xml
6. Use the binarycreator tool to create the installer. 
```
 <location-of-ifw>\binarycreator.exe -t <location-of-ifw>\installerbase.exe -p <package_directory> -c <config_directory>\<config_file>  <installer_name>
 ```

## Deploying the Server
FutureWallet's REST Server runs on Ubuntu using virtualenv, Flask, uWSGI, and Nginx.
To set up the server, you will need to install python3, all dependencies, and the modules/services listed above.
Then, you will need to create a service on the system for the application using systemctl.
Once that is complete, you need to create another configuration file in nginx's sites-available folder.

## Test Build Instructions
Note that Qt and some executable make are required to build the unit tests.
In order to run the unit tests for the BudgetData and Requests classes, do the following:
In the [Test](./Application/Test) directory, run the following commands:
```
qmake -makefile -o ./TestBuild/Makefile "CONFIG+=test" ./BudgetDataTest/Test.pro
make -C ./TestBuild
qmake -makefile -o ./TestBuild/Makefile "CONFIG+=test" ./RequestsTest/RequestsTest.pro
make -C ./TestBuild
python -m flask run &
./TestBuild/Test.exe
./TestBuild/RequestsTest.exe
```
Make sure to run the flask app (mock endpoints) in a separate shell or in the background.
We had to separate the tests because I couldn't figure out how Qt included the QApplication in the main function that they abstractly generate. The Requests class requires the QApplication class for the QEventLoop objects.

Or you can just run [MakeAndRunTest.bat](./Application/Test/MakeAndRunTest.bat) in the [Test](./Application/Test) directory assuming you have the environment variables set up, and the same make version as the developers.

[//]: # (For Windows User dev team, make is C:\Qt\Tools\mingw730_64\bin\mingw32-make.exe)
