# Collaborative document - Build Systems Course and Hackathon, October 2024

## Icebreaker question

What is favourit editor / integrated development environment for editing code?

- vi / vim / neovim: ooooooooo
- emacs: oooo
- nano:
- Visual Studio Code: ooooooooo
- Zed:

## CMake & ENCCS

:::info
**Links for the Build Systems Course**:
- [**Schedule**](https://github.com/PDC-support/build-systems-course)
- [**Introduction to CMake**](https://enccs.github.io/intro-cmake/)
>
**Links for ENCCS**
- [**ENCCS**](https://enccs.se/)
- [**Training Events**](https://enccs.se/events/)
- [**Newsletter**](https://enccs.se/newsletter)
- [**Training materials**](https://hackmd.io/@yonglei/mermaid-enccs-lesson)
- Follow us on [**LinkedIn**](https://www.linkedin.com/company/enccs), [**Twitter**](https://twitter.com/EuroCC_Sweden), and [**YouTube**](https://www.youtube.com/@enccs)

On Dardel, load the CMake module with
```
ml PDC/23.12
ml cmake/3.27.7
cmake --version
```

Download the exercise files by cloning the git repo
```
git clone https://github.com/ENCCS/intro-cmake.git
cd intro-cmake/content/code/
```

### Questions:

- I can't understand what is module scope in `add_library`. Shared is loading the library at run time with generated .so file, static is linking the library into the code, but what is module?
  - From [CMake's docs](https://cmake.org/cmake/help/latest/command/add_library.html#normal-libraries): "A plugin that may not be linked by other targets, but may be dynamically loaded at runtime using dlopen-like functionality." So an addon plugin that might be present or not when program is being run.

- When writing `${outer_${inner_variable}_variable}` is the inner_variable then part of the variable name of the outer variable?
  - Yes. Dereference the variable `${inner_variable}` you will get a string. This string is one part of the `out_STRING_variable`.

- Based on cmake help, if you don't specify scope in add_library command, it might use static or shared. How can I check what scope is used after running a command like add_library(target lib_file.cpp)?
  - By default CMake checks what is the value for [BUILD_SHARED_LIBS-variable](https://cmake.org/cmake/help/latest/variable/BUILD_SHARED_LIBS.html#variable:BUILD_SHARED_LIBS). Default is static. So unless you have set the variable, you can assume it is static.
  - :+1:

- is account.so library supposed to be used outside of the bank target? If not, it should be account.a?
  - no it is a static library and only be used to generate the `bank` executable.
      - I mean that if one installs bank exe, one would also need to install account.so, although only bank is ever gonna use account.so?
        - In the code example, the answer is YES.
        - But if you have two sources and want to use account.so, you can
            - set it to be interfaced if these two source files are located in the folder-subfolder structure
            - if the `account.so` has to be private, you can have another copy of the library and link it to another source file.

- One can set properties of any target also in sub-dir `CMakeLists.txt`, i.e. basically anywhere? Sounds like a bad idea? I probably interpreted that incorrectly :) Can you clarify?
  - For PUBLIC properties, for example the `-ffast-math`, you can declare it anywhere as it is `PUBLIC`. In the code example you can set it in the root `CMakeLists.txt` or the leaf `CMakeLists.txt`
  - For PRIVATE properties, you have to declare it in the `account` folder using the command
    ```
    target_sources(account
      PRIVATE
    account.cpp
    )
    ```
  - Then include add the `add_subdirectory(account)` to tell CMake where to find the static library and what library should be used
    ```
    target_link_libraries(bank
      PRIVATE
        account
      )
    ```

- I tested setting (in `account/CMakeLists.txt`)
    ```
    target_compile_options(bank
        PUBLIC
        "-ffast-math"
    )
    ```
    - and it failed, which is good :) One cannot set target properties, even PUBLIC ones, in random places. The initial misconception was that only targets can have properties.
        - `in random place` I mean that you can use this PUBLIC property (`-ffast-math`) multiple times to generate the executable/libraries.
        - In the `account/CMakeLists.txt`, the compiler flag is only used to generate the account library
            ```
            target_compile_options(account
              PUBLIC
                "-ffast-math"
             )
            ```
        - for the `target_compile_options(bank PUBLIC "-ffast-math")`, if you put it in the root CMake script, it should work. In this command, the compiler is used to obtain the `bank` executable.
        - Still not quite understanding the scope of properties. Adding
          ```
          get_property(foo
            TARGET    account
            PROPERTY  target_compile_options
          )

          message("foo: ${foo}")
          ```
          to the root level CMakeLists.txt does not have access to account target's target_compile_options PUPLIC property (prints empty value). And no errors either.
            - this means that the you did not set the variable `foo`, and you did not get anything when running the `get_property` command.
            - let me debug it and paste the result in minutes

- Regarding graphviz on Dardel, seems it is currently not installed.
  - Might be a module?
  - Check with ``ml spider graphviz`` ?
  - The hits for graphviz with spider are old builds which likely do not work. I am trying to install a newer version with Spack, using the specification`spack install graphviz@8.0.5%gcc@7.5.0 +ghostscript +gtkplus +qt +x`. This will run for an hour so, as Spack builds up a large number of dependency programs and libraries.
  - ... and have a go at it also with EasyBuild.
  - Update: a new build of Graphviz with EasyBuild, but was not functional in runtime. The build attempts with Spack failed so far.

#### Exercise 07
See https://enccs.github.io/intro-cmake/probing/ and scroll down to "Exercise 07"

#### Exercise 08, 09
See https://enccs.github.io/intro-cmake/probing/ and scroll down to "Exercise 08", "Exercise 09"

- How do I test runtime capabilities of MPI programs, which usually require some sort of custom launcher, srun, mpirun, aprun, etc?
  - Good question. On a shared server / supercomputer you typically cannot run srun, mpirun, aprun directly. You need first to request a node or a number of cores on a node(s). On Dardel you can use for instance `salloc -n 4 -t 1:00:00 -A edu24.build -p shared` which will provide you with 4 cores on a shared CPU node (which has 128 cores in total) for 1 hour.
  - How do I tell cmake (check_c_source_runs) to run the program with `srun myapp` instead of `./myapp` ?
  - check the code example here `https://enccs.github.io/intro-cmake/tips-and-tricks/#options-and-flow-control` to see how to enable MPI when you build the system.
  - additional topics can be found at the CMake documents: https://cmake.org/cmake/help/latest/module/FindMPI.html
- I have OpenMP3.1 only (that is what is fount at least). How can I fix that? I did load gcc/14.2.0 at UPPMAX.
  - If I recall correctly, OpenMP support largely depends on the compiler. There isn't much to do than trying to use a newer compiler.
  - If you had that new compiler loaded, then maybe Cmake found your system compiler instead for some reason? Maybe someone else knows better?
  - Yeah gcc/14 should have even openMP5.0 so I'm missing somewhere stating the right gcc version. `cc` should be `gcc` somewhere.
      - ``CMAKE_CXX_COMPILER=`` is the key!
      - This works from commandline: ``$ cmake -D CMAKE_CXX_COMPILER=gcc -S. -Bbuild``
      - ``OpenMP 4.5`` is available for ``gcc >=6.X.X`` !
