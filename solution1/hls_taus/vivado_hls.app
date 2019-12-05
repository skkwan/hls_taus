<AutoPilot:project xmlns:AutoPilot="com.autoesl.autopilot.project" projectType="C/C++" name="hls_taus" top="myproject">
    <files>
        <file name="hls_taus/parameters.h" sc="0" tb="false" cflags=""/>
        <file name="../myproject_test1.cpp" sc="0" tb="1" cflags=" -std=c++0x -Wno-unknown-pragmas"/>
        <file name="hls_taus/myproject.h" sc="0" tb="false" cflags=""/>
        <file name="hls_taus/myproject.cpp" sc="0" tb="false" cflags="-std=c++0x"/>
        <file name="hls_taus/BDT.h" sc="0" tb="false" cflags="-std=c++0x"/>
    </files>
    <solutions>
        <solution name="solution1" status=""/>
    </solutions>
    <Simulation argv="">
        <SimFlow name="csim" setup="false" optimizeCompile="false" clean="false" ldflags="" mflags=""/>
    </Simulation>
</AutoPilot:project>

