
A profile file for the FFMPEGVideoExporter plugin is a properly formatted json file.

It must contain the following fields:
    - "format"
    - "codec"

The following fields are optional:
    - "width" (default value, can later be adjusted in GUI)
    - "height" (default value, can later be adjusted in GUI)
    - "fps" (default value, can later be adjusted in GUI)
    - "seconds" (default value, can later be adjusted in GUI)

    - "gopsize"

An example of a minimal profile.json would look like:

{
    "format": "avi",
    "codec": "mpeg4"
}

An example of a comprehensive profile.json would look like:

{
    "format": "avi",
    "codec": "mpeg4",
    "width": 800,
    "height": 600,
    "fps": 30,
    "seconds": 5,
    "gopsize": 60
}
