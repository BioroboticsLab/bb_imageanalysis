# BeesBook image analysis pipeline for the cray supercomputer

This binary executes the BeesBook image analysis on the cray supercomputer. 

## Dependencies

* Boost
* OpenCV
* MPI

## Setup

### Clone, initialize build directory and compile project

```
git clone git@github.com:BioroboticsLab/bb_imageanalysis.git
mkdir bb_imageanalysis-build
cd bb_imageanalysis-build
cmake -DUSE_CRAY=On ../bb_imageanalysis
make
```

### Usage example
```
git clone git@github.com:BioroboticsLab/deeplocalizer_data.git
git clone git@github.com:BioroboticsLab/deeplocalizer_models.git

./bb_imageanalysis image_path settings_path
```

For settings_path, use for example: ```deeplocalizer_data/images/season_2015/cam0/parameter/settings.json```

As of right now, you have to manually edit the deeplocalizer config path in the settings.json file
to valid absolute paths on your machine:
```
"DEEPLOCALIZER_PARAM_FILE"
"DEEPLOCALIZER_MODEL_FILE"
```

Expected folder structure for n nodes, each processing m images:
```
image_path/
  proc_0/
    image_0.jpeg
    image_1.jpeg
    ...
    image_m.jpeg
  proc_1/
  ...
  proc_n/
```

### Citation
> Wario, Fernando, et al. "Automatic methods for long-term tracking and the detection and decoding of communication dances in honeybees." Frontiers in Ecology and Evolution 3 (2015): 103.
