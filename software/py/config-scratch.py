project = {
    "name":"ArtWall",
    "lightgroups":
    [ 
        { 
        "name": "moon", 
        "connection": "usb",
        "num_boards":2,
        "lights": 
            [ 
                {   
                "name": "left", 
                "boardnum":0,
                "index": 0,
                "loc":[[0,0,0],[1,1,1]], 
                "channels": [0,1,2]
                },
                {   
                "name": "right", 
                "boardnum":0,
                "index": 1,
                "loc":[[5,0,0],[6,1,1]], 
                "channels": [3,4,5]
                }

            ]
        }

    ]
}
con