from Parser import CMakeCacheParser
import pytest
import os

def test_find_file(tmpdir, request):
    p = CMakeCacheParser.CMakeCacheParser()

    with pytest.raises(RuntimeError):
        p.parse()

    testFile = os.path.join(request.fspath.dirname, "CMakeCache.txt")
    q = CMakeCacheParser.CMakeCacheParser(testFile)
    q.parse()

    assert q.getDependencies("CEFile") == ["ScaleUtilsAmpxLib_fortranlib", "ScaleUtilsAmpxLib", "ScaleUtils_IO", "Standard_fboff", "Standard_boff", "Interface"]

    allDeps = q.getDependencies()
    assert allDeps[4] == ["CEFile", ["ScaleUtilsAmpxLib_fortranlib", "ScaleUtilsAmpxLib", "ScaleUtils_IO", "Standard_fboff", "Standard_boff", "Interface"] ]