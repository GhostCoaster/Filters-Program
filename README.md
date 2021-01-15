# Filters-Program

## A command line utility to manipulate images with "filters"
*(you probably wouldn't actually want to do this)*

| Flag | Name | Usage |
|:----:| ---- | ----- |
|-i    |Image |Any number of strings, each a filepath to an image to be filtered|
|-f    |Filter|First, the name of the filter, followed by parameters if supported|
|-s    |Suffux|Changes the suffix of the output filenames       |
|-o    |Output|Changes the firectory of the output files       |

Example:
```Filters_Program -i "haruka.png" -f blur 17```

Tips:
* The -i flag can be omitted if it is the first argument
* You can have any number of -f flags
* Use `true` or `false` or any substring for boolean filter parameters
* All filter parameters have a default value
