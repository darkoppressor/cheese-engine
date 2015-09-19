@echo off

set PROJECT_DIRECTORY=%1
set TARGET=%2
set ACTION=%3

if not [%PROJECT_DIRECTORY%] == [] (
	if not [%TARGET%] == [] (
		if not "%TARGET%" == "release" (
			if not "%TARGET%" == "debug" (
				echo "TARGET must be either 'release' or 'debug'"

				goto :EOF
			)
		)

		if not [%ACTION%] == [] (
			if not "%ACTION%" == "clean" (
				if not "%ACTION%" == "rebuild" (
					if not "%ACTION%" == "install" (
						echo "ACTION must be either '', 'clean', 'rebuild', or 'install'"

						goto :EOF
					)
				)
			)
		)

		cd "%PROJECT_DIRECTORY%\development\android"

		if [%ACTION%] == [] (
			C:\Development\android-ndk\apache-ant\bin\ant.bat %TARGET%

			goto :EOF
		)

		if "%ACTION%" == "rebuild" (
			C:\Development\android-ndk\apache-ant\bin\ant.bat %TARGET% clean
			C:\Development\android-ndk\apache-ant\bin\ant.bat %TARGET% build

			goto :EOF
		)

		C:\Development\android-ndk\apache-ant\bin\ant.bat %TARGET% %ACTION%
	) else (
		echo "ant - run ant on the passed project"
		echo "Usage: ant PROJECT-DIRECTORY TARGET ACTION"
		echo "TARGET must be either 'release' or 'debug'"
		echo "ACTION must be either '', 'clean', 'rebuild', or 'install'"
	)
) else (
	echo "ant - run ant on the passed project"
	echo "Usage: ant PROJECT-DIRECTORY TARGET ACTION"
	echo "TARGET must be either 'release' or 'debug'"
	echo "ACTION must be either '', 'clean', 'rebuild', or 'install'"
)
