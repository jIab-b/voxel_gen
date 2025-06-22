import os

compiler = "g++"
source_files = ["main.cpp", "world.cpp", "world_generation.cpp", "meshing.cpp", "player.cpp"]
output_file = "main.exe"
include_paths = ["-Iinclude"]
library_paths = ["-Llib"]
libraries = ["-lraylib", "-lopengl32", "-lgdi32", "-lwinmm"]
defines = ["-DPLATFORM_DESKTOP"]

command = f"{compiler} {' '.join(source_files)} -o {output_file} {' '.join(include_paths)} {' '.join(library_paths)} {' '.join(libraries)} {' '.join(defines)}"

print(f"Executing: {command}")
os.system(command)