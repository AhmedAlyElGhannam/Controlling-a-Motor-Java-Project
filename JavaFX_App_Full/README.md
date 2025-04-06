# Creating a Desktop Icon For JavaFX App
1. Create a `.desktop` file like the one located in this directory. Make sure to replace the full path written in it with your own. If you want the app to have an icon, make sure it is small enough and to include its full path as well. **the full path shenanigans even extends to where the `java` bin is located so its full path has to be written as well.**
1. Package all your `.java` and their `.jar` dependencies into one `.jar`. Simply run `make package`.
1. Copy your `.desktop` file to `~/.local/share/applications/`.
1. Make the `.desktop` file executable.
1. Profit!
