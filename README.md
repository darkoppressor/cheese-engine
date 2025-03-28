# cheese-engine

[![Build Status](https://wells-family.xyz/jenkins/buildStatus/icon?job=cheese-engine)](https://wells-family.xyz/jenkins/job/cheese-engine/)

2D game engine for Windows, macOS, Linux, and Android
https://cheeseandbacon.org/

The Cheese Engine is the engine running Cheese and Bacon Games', uh, games.
It was developed in-house (literally, I don't have an office) by intrepid programmer Kevin Wells. That's me.
It is probably a horrible engine. It began years ago as a game written by me while I was still learning to write code.
This game was never completed. It did, however, become another game.
That game was completed, and it was called Hubert's Island Adventure: Mouse o' War. The title of the game was long.
Since that time, more projects and games were written with the same code, copy and pasted again and again.
Modifications were made, limbs were grafted on, pieces were removed.
Finally, to end the madness, I ripped all of the engine-specific stuff out of my most recent project and turned it into
a software library. That software library is the Cheese Engine. It is written in C++, with a little C and Java here and
there. Peruse the source code if you dare, but be warned: No one but me has ever used this for anything.

## Using the engine

* [Asset guidelines](docs/assetGuidelines.md)
* [Animation guidelines](docs/animationGuidelines.md)
* [GUI guidelines](docs/guiGuidelines.md)

* [Text files](docs/textFiles.md)
* [Versioning](docs/versioning.md)
* [Workflow](docs/workflow.md)

* [Style guide](docs/styleGuide.md)

* [Building the Cheese Engine for Android for debug](docs/buildingAndroidDebug.md)

* [Template instructions](docs/templateInstructions.md)
* [Build instructions](docs/buildInstructions.md)

## Developing the engine

* [To do](development/toDo.md)

* [Ideas](development/design/ideas.md)

### Updating the engine version

In [src/engine_version.cpp](src/engine_version.cpp):

    Update the version returned by Engine_Version::get_engine_version
    Update the date returned by Engine_Version::get_engine_date

When an updated version has been pushed to main, tag the commit locally and remotely:

    git tag <VERSION> -m "<VERSION>"
    git push origin tag <VERSION>

## License

This engine's source code is licensed under the MIT License. See [LICENSE.md](docs/LICENSE.md) for the full license
text.

Please note that there are a couple of files in the docs/ directory named contentLicense<Type>.md
These are for use with projects using the engine, and do not pertain to the engine itself.

For third party licenses for dependencies, see [the thirdPartyLicenses directory](docs/thirdPartyLicenses).
