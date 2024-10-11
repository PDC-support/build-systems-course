# Collaborative document - Build Systems Course and Hackathon, October 2024


## :icecream: Icebreaker question

### What is your favourite drink when programming?

- coffee: oooooo
- tea: oooooo
- coke: o
- pepsi: o
- Hot chocolate: oo

## Spack

* Slides for session 1: https://github.com/PDC-support/build-systems-course/blob/main/presentations/Spack/SpackLecture1.pdf
    * Slides of the official Spack tutorial: https://spack-tutorial.readthedocs.io/en/latest/
* Command line log for the demo session 2: https://github.com/PDC-support/build-systems-course/blob/main/presentations/Spack/spack-demo.md

### Questions:

**INFO**: Note that `–I` in the pdf will be wrong dash. You may need to replace it with ordinary dash `-I`

**Question 1:**
I always wondered how could one know beforehand the libraries a certain package will install, without actually installing the package. This is very useful to avoid installing software that probably one won't need in the end. Something like `spack spec` would do, but unfortunately that method does not print out the libraries that would be installed for that certain package. Let me guide you through this minimal working example:
- Let's say that I want to install the `libpmi2.so` library. The following package:  `pmix` should contain it, and this package can of course be found in spack [here](https://packages.spack.io/package.html?name=pmix)
- Now, the `spec` method, i.e. `spack spec pmix` won't show the libraries that would be installed. 
- **Is there a way to know the libraries a certain package would install without actually installing the package?**
    - PL: You mean the individual files, like "libxxx.a". I think not, because Spack does not work like e.g. RPM.
- **Neither `spack spec pmix` nor `spack spec -I pmix` show the libraries.**
    - Quite often one finds the software that should provide the headers / libraries via Googling or by running something like `yum whatprovides *libpmi2.so` on the cluster to find the system package. Then one can find the corresponding Spack package and install that. In vast majority of cases the dependencies for a software are given in a README/installation instructions etc. for the software so just installing the dependencies should be enough and knowing exact library versions is not needed and can even be detrimental for portability.

- **There is no `yum` in dardel...** **Is it possible to get it installed?**

- Ideally, one would like to use `spack` to show the libraries that `spack install <package>` would install for that `<package>`. One is not that interested in the version of the library itself, but however the actual library.
    - Spack does not know beforehand what files a package installs because different packages use different installation methods (CMake/Autotools) and different variants can provide different artifacts at the end. Getting SBOM (Software Bill of Materials) for all installed files is a complicated question and [there are some examples](https://github.com/spack/spack-sbom) on getting this, but usually the exact library names are not known beforehand. Typically software needs e.g. BLAS libraries, but the headers and libraries are found with PkgConfig or CMake, so the user does not necessarily need to know whether `libmkl*.so` or `libOpenBlas*.so` provides the BLAS implementation. 
        - But I do agree that sort of feature would be fantastic.

**Question2:**
**On a system such as Dardel, you typically want to use the MPI library of the system rather than some MPI library in Spack. Is there an easy way to achieve this when installing an MPI dependent package?**

- You can specify the MPI package as an external package. This makes Spack to use the package for MPI installation. See  [this documentation page](https://spack.readthedocs.io/en/latest/packages_yaml.html#external-packages) on specifying external packages. You can even get the packages via module system.
- Is this true also for compilers?
  - Yes. On e.g. the Dardel machine at PDC, which is a Cray EX, we have the Spack installation configured to use Cray MPICH, the Cray compilers, and the Cray LibSci library by default, but you can choose a different compiler by e.g. `spack install quantumespresso %gcc`
- What if it does not find the specific version of MPI? Will it complain it does the find the lib or will it install that particular version? Or, better, change the version of MPI it depends on?
  - If there exists an external package that provides everything needed by the spack spec, spack will use that package during the spec phase. If the MPI has not been specified to be provided by an external package it will try to install a version that satisfies the spec.
- I already had spack installed. Trying to update with git pull. Hope that is the best way!
  - That is usually the best way. If you want to increase the reproducibility you can checkout one of the spack releases e.g. `git checkout releases/latest`. This can make it easier to know which version of spack you used to install your software. 
      - Nice!
  - ``HOME`` dir became full so better with project dirs for our users!
- FYI: There also exists a project called [e4s.io](https://e4s-project.github.io) that maintains a huge environment that has a lot of programs pre-installed for supercomputers. Build cache is [here](https://oaciss.uoregon.edu/e4s/inventory.html)
- So, the HPC user can use spack, yes, but how will the created module work with the existing lmod module system, just shown for the user who installed it locally? Or I missed something!?
    - The installed module will end up in the spack environment installation path or spack's installation folder. It will be present to the user. Of course mixing and matching software from the cluster and from spack can cause complications if spack did not use that software while building. If you notice that both spack and system module try to load some software, you can add the system module as an external software to spack so that it knows of its existence.
    - You can also specify in [modules configuration](https://spack.readthedocs.io/en/latest/module_file_support.html#the-modules-yaml-config-file-and-module-sets)  where you want to install the module files.
- Has anyone tried installing with spack and make it an lmod module? Perhaps the answer above answers!
    - You can install a spack package and let it create lmod modules. See the configuration page on the previous answer. After installing you can run `module use /path/to/spack/share/spack/lmod` (or your custom install location) to make the modules available.
    - PL: If you are asking about whether you can make an lmod module for Spack that initialize and set up the environment, like `module load spack`, and then get a Spack installation, it is sort of possible, but requires some hacking. The spack init script does many things which are not so easy to reproduce in a module file. We have this on Dardel at PDC and on LUMI.
  - Great!

- Can I activate spack environment in any directory or first I need to change my directory to where the env directory exists?
  - You can do it in any directory as long as spack is sourced to the terminal.

- Seems that I got many problems with depracated stuff. Was that the reason for using 0.22 for mirror instead of 0.23?
    - "==> Warning: The config:install_missing_compilers option has been deprecated in Spack v0.23, and is currently ignored. It will be removed from config in Spack v0.25.
==> Warning: Spack supports only clingo as a concretizer from v0.23. The config:concretizer config option is ignored.
==> Error: The "config" section of the configuration needs to be written to disk, but is currently using a deprecated format. Please update it using:

        spack config [--scope=<scope>] update config

Note that previous versions of Spack will not be able to use the updated configuration."
  - ``spack.0.23.0/bin/spack`` (just the clone)
  - PL: 0.22 is the latest official release. If you check out the git repo, you end up in the development branch automatically, which is closer to 0.23 release. I think that they have not built a binary cache for 0.23 yet.
  - PL: the install missing compilers error is harmless for now. Are you using the PDC Spack module on Dardel? There are some stuff there that I still need to fix. Generelly, the errors that you get is related to old syntax in the config.yaml file in spack/etc.
  - BC: No UPPMAX. It worked rather OK with my last version. I have old ``~/.spack`` configs for like v.0.16 or so
      - PL: Yeah, that might be the reason. I often find that I have to remove the settings in $HOME/.spack, because it contains old stuff that does not work with a new spack version. The way I use Spack is that I disable the home directory settings, and put the config files in the spack/ directory instead. `export SPACK_DISABLE_LOCAL_CONFIG=1`
    - Good. I will upgrade our docs with some good-to-know and best procedure things! Many thanks!

## EasyBuild
https://github.com/PDC-support/build-systems-course/blob/main/presentations/easybuild/easybuild.pdf

### Questions

- Does ``~/.local/easybuild/`` become cluttered with many small files and the full installations?
  - Typically not. For most programs the final result of an easybuild are files in bin, include, and lib directories. Intermediate files for the build are not kept
    - OK, But IF we let users on like UPPMAX make their own installations we should make sure that they define the build dirs in their project area. 
- When working on LUMI how do I know if I should use spack or easybuild?
  - PL: EasyBuild is the officially supported solution. You can get help from the LUMI Support team using EasyBuild. We try to maintain a Spack installation that works, but there might be limited help available. The way you use Spack is typically that you install it yourself as a user.
  - Otherwise: Spack is better if you are a software developer. EasyBuild is easier if you just find a good supported installation recipe.  I would say it is also easier to make a basic EasyBuild recipe for software, compared to a Spack package, especially if you do not know Python. If you know Python, it is straightforward.
