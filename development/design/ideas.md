# Ideas

Linux Cheese Engine on Steam Deck defaults to touch controls, but the buttons work
Can the touch controls default to off for Steam Deck?
Ideally in a way that isn't actually specific to Steam Deck
Note this is not the Steam build of the game
That version might have a simple way to detect that we are on a Steam Deck, I dunno

SDL_OpenURL can probably replace my current URL opening function
Can I use the new SDL2 SDL_GameControllerRumble to replace the current rumble implementation?

Scroll bar for scrolling buttons and text

Unicode support in bitmap fonts?

Enable joining a game in progress

IPv6

Log could have an option to add the log to a text file alongside the error one

The whole engine should use const references wherever possible

***
Multithreading:
The game startup process could be partially multithreaded
The game normally sequentially loads all assets while displaying a load screen (as soon as it can)
When done, you are on the menu and input is working
What if I added a special class of assets, whose names are defined in a script
(or maybe placed in one or more subdirectories)
Any listed asset will be skipped over during initial loading
At this point, the game should have a list of all special assets, but they won't be loaded yet
Once initial loading is complete, the main menu is shown per normal
At this point, the game begins loading the special assets MT in the background
The engine would have a boolean that represents whether this secondary loading is done yet or not
Anything that requires any special assets would need to be unavailable until loading is done
When trying to access an asset, if it doesn't exist but is on the special list,
the engine should return a special error explaining this
This would make it easier to catch crashes caused by misusing this feature
What would this ultimately gain? It would be up to the game
My first thought is that you would make your big music files special,
and this would just allow navigating the menus much more quickly rather than waiting on loading
In fact, with music, it would be possible to allow gameplay to begin and just start playing music as soon as it is done loading
Worst case, you couldn't do anything beyond the main menu and its submenus
(except for starting the game), but this still seems like a small gain
It would mainly be useful for something more like the music example above though
Perhaps this could be made more sophisticated, and make each special asset available as soon as it is loaded,
not requiring the entire special asset loading to complete before using some of them
***

Implement speed of sound
A constant should give meters per pixel, to relate equations to the game world
Another constant should give the actual speed of sound in meters/second
"Played" sounds would thus need to be added to a queue and only actually played when the player should hear them, with the delay being calculated based on the distance between the sound and the player using the speed of sound
Only problem here is that this would only work if all materials in the game have the same speed of sound
I think it is fine to just leave this for now
I can expand further if I ever make a game where this distinction in important

Can the sound volume falloff be based on some actual physics thing and made more realistic?

Implement the Doppler Effect
I think this would require something more than SDL2_Mixer for sounds, so I could change the pitch of them to implement this effect

Add text colors to buttons, windows, tooltips, and toasts

Write up guidelines for contributors

Write documentation for using the engine, from a non-code standpoint

Dedicated server mode (can be implemented as a headless mode, controlled by a command line argument)

String data / datafiles
These can be referenced in my other data files in some special way
These can also be referenced in code (this should be really simple to do; look at how colors work?)
Engine needs a language option (this will be a user-configurable option) (defaults to english)
String data entries are now children within a language data entry
When retrieving a string, retrieve the string from the currently set language
With this all setup, this might enable my other idea for making error messages a data file thing
I would also need to extend my stuff to use unicode or whatever instead of just ascii

Change to using dynamic linking instead of static?

Add SDL2 audio capture support
Add android mic recording support
Add audio comparison code
Could be used in say bit beast for calling your pet by name
