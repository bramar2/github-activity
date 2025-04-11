# github-activity: A CLI tool for fetching Github Activity (Windows)
[https://roadmap.sh/projects/github-user-activity](https://roadmap.sh/projects/github-user-activity)
## Code
The project uses CMake and C++23 standard. Uses modified bramar2/json library put directly into this project. Windows only.
## Building
On root, run the following commands:
- `cmake -S . -B build/`
- `cmake --build build/`


The executable should be in build/github-activity.exe.

## Preview
```
usage: github-activity user <username>
usage: github-activity repo <repo>
usage: github-activity network <network>
usage: github-activity org <organization>
usage: github-activity public
```
![image](https://github.com/user-attachments/assets/6f189d38-63e6-410f-ae59-f97cf215e16f)
