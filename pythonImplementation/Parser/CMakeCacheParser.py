import re

class CMakeCacheParser:
    def __init__(self, filePath=None):
        self.file = filePath

    def printDependencies(self):
        for item in self.dependencies:
            print(item[0]+":")
            for dep in item[1]:
                print("  "+dep)

    def getDependencies(self, name=None):
        if name is None:
            return self.dependencies
            
        for item in self.dependencies:
            if item[0] == name:
                return item[1]

        raise RuntimeError("Package not found in CMakeCache: "+name)

    def parse(self):
        if self.file is None:
            raise RuntimeError("No CMakeCache Filepath specified")
        
        # Find all instances of xxxSomeLibxxx_LIB_DEPENDS
        dep = []
        with open(self.file) as f:
            for line in f:
                match = re.search('LIB_DEPENDS', line)
                if match is not None:
                    dep.append(line.strip())

        # Lib depends lines in CMakeCache have the following format:
        # {libName}_LIB_DEPENDS:STATIC=general;{dep1};geneal;{dep2};general;...;general;{depN};

        # Split lines by :STATIC=
        dep2 = []
        for item in dep:
            dep2.append(item.split(":STATIC="))

        # Split dependencies by ;general;
        dep3 = []
        for item in dep2:
            dep3.append([item[0], item[1].split(";general;")])

        # Clean library name
        for item in dep3:
            item[0] = item[0][0:-12]
            item[1][0] = item[1][0][8:]
            item[1][:] = item[1][:][0:-1]

        # Store the resolved dependencies in a table
        self.dependencies = dep3