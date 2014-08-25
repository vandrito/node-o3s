{
  "targets": [
    {
      "target_name": "nodeo3s",
      "defines": [ "_O3SCOREDICO_API_" ],
      "sources": [ "src/nodeo3s.cpp", "src/resolver.cpp", "src/dico.cpp" ],
      "include_dirs": [ 
		     "/opt/o3s/tools/framework/include/",
		     "/opt/o3s/tools/framework/include/O3S",
         "/opt/o3s/tools/framework/include/O3S/Object",
		     "/opt/o3s/tools/framework/include/D3S",
		     "/opt/o3s/tools/framework/include/Poco",
		     "/opt/o3s/tools/framework/include/Poco/XML",
		     "/opt/o3s/tools/framework/include/Poco/Util",
		     "/opt/o3s/tools/framework/include/Poco/Net",
		     "/opt/o3s/tools/framework/include/Poco/Remoting/JSON",
		     "/opt/o3s/tools/framework/include/Poco/Net/DNSSD/",
         "/opt/o3s/tools/framework/include/Poco/Net/DNSSD/Avahi"],
      "cflags_cc": [ "-frtti", "-fexceptions", "-std=c++11", "-fPIC" ],
      "conditions": [
    	  ["OS=='mac'", {
    	    "xcode_settings": {"GCC_ENABLE_CPP_EXCEPTIONS": "YES"}
    	   }],

        ["OS=='linux'", {
          "link_settings": {
              "libraries": [
                  "-lo3sdico", "-ld3s", "-lo3s.foundation", "-lo3s.xml", "-lo3s.util", "-lo3s.net", "-lo3s.remoting", "-lo3s.net.dnssd", "-lo3s.net.dnssd.avahi", "-L/opt/o3s/runtime/common"
              ]
          }
         }]
      ]
    }
  ]
}
