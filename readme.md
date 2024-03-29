<a href="https://www.microchip.com"><p align="left"><img src="./docs/microchip.png" width="350" alt=""></a>

# Microchip SWI Logic Analyzer

This protocol analyzer is built with [SWI EEPROMs](https://www.microchip.com/en-us/parametric-search.html/506?filters=eyJjYXRlZ29yeWRyb3Bkb3duIjpbIk1pY3JvY29udHJvbGxlcnMgYW5kIE1pY3JvcHJvY2Vzc29ycyIsIkFsbCBNaWNyb2NvbnRyb2xsZXJzIiwiIl19) in mind, this offers a consistent readout of SWI protocol.

Documentation for the Saleae Logic Analyzer SDK can be found here:
https://github.com/saleae/SampleAnalyzer

That documentation includes:

- Detailed build instructions
- Debugging instructions
- Documentation for CI builds
- Details on creating your own custom analyzer plugin

# Installation Instructions

To use this analyzer, simply download the latest release zip file from this Git Hub repository, unzip it, and then install using the instructions found here:

https://support.saleae.com/faq/technical-faq/setting-up-developer-directory

> [!NOTE]  
> The following section only applies to downloaded pre-built protocol analyzer binaries on MacOS. If you build the protocol analyzer locally; or acquire it in a different way, this section does not apply.

Any time you download a binary from the internet on a Mac, whether it be an application or a shared library, MacOS will flag that binary for "quarantine". MacOS then requires any quarantined binary to be signed and notarized through the MacOS developer program before it will allow that binary to be executed.

Because of this, when you download a pre-compiled protocol analyzer plugin from the internet and try to load it in the Saleae software, you will most likely see an error message like this:

> "SWIAnalyzer.so" cannot be opened because the developer cannot be verified.

Signing and notarizing of open source software can be rare; because it requires an active paid subscription to the MacOS developer program, and the signing and notarization process frequently changes and becomes more restrictive, requiring frequent updates to the build process.

The quickest solution to this is to simply remove the quarantine flag added by MacOS using a simple command line tool.

Note - the purpose of code signing and notarization is to help end users be sure that the binary they downloaded did indeed come from the original publisher and hasn't been modified. Saleae does not create, control, or review 3rd party analyzer plugins available on the internet, and thus you must trust the original author and the website where you are downloading the plugin. (This applies to all software you've ever downloaded, essentially.)

To remove the quarantine flag on MacOS, you can simply open the terminal and navigate to the directory containing the downloaded shared library.

This will show what flags are present on the binary:

```sh
xattr libSimpleSerialAnalyzer.so
# example output:
# com.apple.macl
# com.apple.quarantine
```

This command will remove the quarantine flag:

```sh
xattr -r -d com.apple.quarantine libSimpleSerialAnalyzer.so
```

To verify the flag was removed, run the first command again and verify the quarantine flag is no longer present.

## Building your Analyzer

> [!NOTE]  
> This section if for those with compatability issues or those who wish to modify the analyzer for their own purposes.

CMake and a C++ compiler are required. Instructions for installing dependencies can be found here:
https://github.com/saleae/SampleAnalyzer

The fastest way to use this analyzer is to download a release from Git Hub. Local building should only be needed for making your own changes to the analyzer source.

### Windows

```bat
mkdir build
cd build
cmake .. -A x64
cmake --build .
:: built analyzer will be located at SampleAnalyzer\build\Analyzers\Debug\SimpleSerialAnalyzer.dll
```

### MacOS

```bash
mkdir build
cd build
cmake ..
cmake --build .
# built analyzer will be located at SampleAnalyzer/build/Analyzers/libSimpleSerialAnalyzer.so
```

### Linux

```bash
mkdir build
cd build
cmake ..
cmake --build .
# built analyzer will be located at SampleAnalyzer/build/Analyzers/libSimpleSerialAnalyzer.so
```
