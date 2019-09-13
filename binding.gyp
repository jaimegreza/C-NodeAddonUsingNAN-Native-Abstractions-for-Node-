{
    "targets": [{
        "target_name": "secret_keeper_lib",
        "cflags!": [ "-fno-exceptions", "-g", "-std=c++14"],
        "cflags_cc!": [ "-fno-exceptions", "-g", "-std=c++14"],
        "sources": [
            "./Game.cpp",
            "./GameManager.cpp",
            "./SecretKeeper.cpp",
            "./secret_keeper_addon.cc"
        ],
        'include_dirs': [
            "<!@(node -p \"require('nan').include\")",
            "/opt/local/var/macports/software/curl/7.19.6_0/opt/local/include"
        ],
        'libraries': ["-lcurl", "-L/opt/local/var/macports/software/curl/7.19.6_0/opt/local/lib"]
    }
    ]
}