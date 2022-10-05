Thanks for a great semester. 

**To run**
```
Clone repo into folder
Open folder in Visal Studio
Generate CMake cache
Select Blockbuster.exe from startup items
Make sure configuration is set to x64-release with RelDelWithInfo (Right of the build button)
Build and Run Blockbuster.exe
```

# Keybinds and actions
## translations
**space** - go downwards

**arrowUp** - go forwards

**arrowDown** - go backwards

**arrowRight** - go right	

**arrowLeft** - go left

## Rotations

 **e** - positiveYaw

 **d** - negativeYaw	 

 **w** - positiveRoll	

 **s** - negativeRoll	

 **q** - positivePitch	

 **a** - negativePitch



## Bugs

Initial block might instantiate with fewer blocks. This will only happen for the first block and is most likely to happen if your pc is under strain. (I think it might also only be if the program is doing recompilation due to changes done since last build) As it seem to not happen if it is already compiled and built before running. It is undeterministic as you can run the program, have it happen, then run it again and not get this bug. 

This is the only bug, even though it is odd. It is miniscule as all the next blocks will have its real shape, and the missing block doesnt create any invisible block, it just does not exist. Rotations and translations for the pieces existing still works properly.
