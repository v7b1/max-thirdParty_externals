import os

#path = '/Users/vb/Documents/Max 8/Packages/vb.mi-dev/source/mutableSource/plaits/'

path = '/Users/vb/Documents/coding/max-externals/fluidsynth/source/projects/fluidsynth_tilde/src'

files = []

# r=root, d=directories, f = files
for r, d, f in os.walk(path):
#    for dir in d:
#        print(dir)
    for file in f:
        print('"${SRC_PATH}/'+file+'"')
        files.append(os.path.join(r, file))

#for f in files:
 #   print(f)
