# Demo Part 1 - Install Spack and Build HPL

Commands:

    git clone -c feature.manyFiles=true --depth=2 https://github.com/spack/spack.git spack
    . spack/share/spack/setup-env.sh
    which spack
    spack arch
    spack find
    spack compilers
    spack compiler find
    cat /home/pla/.spack/linux/compilers.yaml

Add a binary cache:

    spack mirror add v0.22.2 https://binaries.spack.io/v0.22.2
    spack buildcache keys --install --trust

Install HPL:

    spack spec -I hpl+openmp ^mpich ^openblas
    spack install hpl+openmp ^mpich ^openblas

Check the installation:

    spack find
    ls /home/pla/tmp/spack.0.23.0/opt/spack/linux-ubuntu22.04-zen2/gcc-12.3.0/hpl-2.3-7pwzwahf4rueovfw7apck2lqavon4xr6
    module avail
    spack load hpl
    which xhpl
    echo $PATH

Run HPL!

    cp ~/code/HPL.dat .
    spack load mpich
    mpiexec.hydra -n 4 xhpl

You can search for packages...

    spack info alphafold   (this will fail)
    spack list alphafold
    spack info py-alphafold
    spack info gcc
    spack info openfoam

# Demo Part 2 - Make a Spack environment

(Also ca. 20-22 minutes)

I have a small (ChatGPT-generated) test program that uses BLAS + FFTW. It builds with CMake.

    cd ~/code/buildcourse/blas
    cd src
    cat CMakeLists.txt

How would use Spack to install the libraries that you need and integrate it with the build system?

The `spack.yaml` file defines an **environment** consisting of all the libraries/packages that you need. We need the `fftw` and `openblas` packages only.

    spack:
        # add package specs to the `specs` list
        specs: [fftw~mpi,openblas]
        view: true
        concretizer:
            unify: true

Create the environment and activate it:

    spack env create -d myenv spack.yaml
    spack env activate myenv

The `spack find` command will now give a different output: "0 installed packages". We need to build the environment (ca 2-3 mins with build cache)

    spack concretize
    spack install
    spack find

Now we load the required libraries:

    spack load openblas
    spack load fftw

Let us build. Make a build directory for CMake

    cd ..
    mkdir build
    cd build
    cmake ../src

The output should say

    Found fftw3, version 3.3.10
    Found BLAS: ...

Build

    make
    ldd fftblas
    ./fftblas

(Bonus: look at the contents of the view directory. `myenv/.spack-env/view`)

