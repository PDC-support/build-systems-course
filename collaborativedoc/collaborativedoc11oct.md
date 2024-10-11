# Collaborative document - Build Systems Course and Hackathon, October 2024


## :icecream: Icebreaker question


### Have you experienced the "works on my machine, sorry" problem?

What was then the problem and what was the solution?
- Some forgotten configuration of the program
- type the wrong commands, :laughing:
- different programming environement
- different program/dependency versions
- ...
- ...


### How is the weather at your place?

- Tromso: not nice, raining and feels like 3 degrees
- Stockholm: raining outside, not inside
    - Värmdö: foggy
    - bandhagen: raining
- Oslo: FINALLY sunny!!!
- Helsinki: pretty nice autumn weather (soon it will rain though)
- Trondheim: blue sky, and the sun is on the rise :smile:
- Espoo: Cloudy and autumny
- Uppsala: cloudy


## Containers - lesson material

(all times here in CEST)

- 09:00 – 09:50 [Overview](https://coderefinery.github.io/hpc-containers/intro_and_motivation/) and [building containers](https://coderefinery.github.io/hpc-containers/building_images/) (Jaan Tollander de Balsch)
- 09:50 – 10:00 Break
- 10:00 – 10:50 Containers and [MPI](https://coderefinery.github.io/hpc-containers/mpi/)/[GPU](https://coderefinery.github.io/hpc-containers/gpus/) (Simo Tuomisto)
- 10:50 – 11:00 Break
- 11:00 – 11:45 [Best practices for sharing reproducible containers](https://coderefinery.github.io/hpc-containers/sharing/) (Maiken Pedersen and Radovan Bast)
- 11:45 – 12:00 Wrap up of lesson & concluding remarks



## Questions

### Intro to containers
https://coderefinery.github.io/hpc-containers/intro_and_motivation/

### Basics of running containers
https://coderefinery.github.io/hpc-containers/basics_running_containers/


- Does the container contain the OS?
    - Yes. Containers usually contain a Linux distribution. However the operating system is not launched in a same way as a computer launches. Instead the linux kernel makes it possible to use the files inside the operating system without starting it. So library dependencies come from the container OS when container runs an application, but no windowing system etc. will be launched.

- What about compilation for native hardware? like -march=native?
    - Software can be built with heavy optimizations, but often software in containers is built with some architecture flags that match the intended place of running. If software is built with very heavy optimizations it might not run in a different place (similar to any other software) because the CPU does not support some instructions. So it might be better to compile for some more generic architecture with reasonable flags (e.g. assume that the CPU supports AVX2, but do not optimize for this specific CPU). So there is no machine translation happening with containers.

- How do we load apptainer/singularity on Dardel?
    - try: `module avail singularity` and it should show all available modules. which you can then load with `module load`. example: `module load singularity/4.1.1-cpeGNU-23.03`

- what is Docker?
    - it is an alternative technology to apptainer/singularity. the one difference to apptainer is that docker is less suitable as container technology on systems with many users (like high-performance computing clusters)
    - Docker and its followers (kubernetes, podman, condainerd etc.) are technologies that power containers that are used for all kinds of applications in cloud etc. These technologies are great, but they require additional permissions that cannot be given in shared systems and thus we focus on Apptainer in this coures. Apptainer is designed for HPC situations.
    - I think also that Docker is the first (?) very successfull such technology and thus lots of Docker images exist out there for reuse.
        - This is also the reason why container images are often built from files called `Dockerfile`. The technology was so popular that this naming convention is used by other container technologies as well.

- When you talk about layers, what are these?
  - Traditional Docker files are stored in layers (like paint layers on an image). When these images are created each command will create a new layer. This is done so that many different images can use same base layers as their starting point which reduces space and network transfer needs.  Apptainer squashes these layers into a one file (think of taking a printed copy of a painting).
       - I like the paint layers analogy! Thanks!

- If you try this on an ARM-chip MacOS, you will have problems with Singularity. Crafting the container iside a running container environment locally works better (using e.g. podman).
    - Apptainer indeed only supports machines with Linux kernel. When developing on Mac docker can be the easier option. Docker is however usually not available on shared systems because it requires additional privileges that cannot be given to normal users.

- If you do not have apptainer installed on your Linux machine, you can try these instructions to install unprivileged installation: https://apptainer.org/docs/admin/main/installation.html#install-unprivileged-from-pre-built-binaries

- What was the command to get that docker//python example?
     - https://coderefinery.github.io/hpc-containers/basics_running_containers/#obtaining-a-container-from-container-registry
     - in short: `apptainer pull python.sif docker://python`

### Container images
https://coderefinery.github.io/hpc-containers/container_images/

- Do I understand correctly that one can reach different "commits" of the build-up of the container?
    - in docker: yes. but in apptainer/singularity at the end we only get one image but not the individual layers. but we can use images as kind-of layers.
    - Indeed this is the case, but if you change something from the previous layer you then have to rebuild all layers that come after that layer.
    - Thanx!

- What about during the development before saving the image?
    - I am not sure, I have never tried to access those layers directly - anyone knows?
    - In docker / podman you can launch any layer as a container based on its hash code. The final product layer is usually just a **tagged** layer that has a human readable name. In apptainer / singularity you can modify the build procedure to stop at some point and store the build into a .sif file and then access it.
      - ah neat! do you have an example for how to add such "breakpoints" in apptainer?
          - I'll add this to the docs later: https://gist.github.com/simo-tuomisto/a83a0ded5c7bc35189842c325ff07715
             - awesome!
               - this is very useful. I thought there was a way to also save a sif "half way" but what you describe is the way I also layer images.
    - Thanx!

- I did not follow: Does
  ```
  Bootstrap: docker
  From: python:latest
  ```
  take the downloaded sif file or fetch from docker .
     - it will try to download from docker and compare the checksum with locally cached images. I believe it will not have to download and translate it again if it already exists locally. but it won't use the python.sif file we created earlier but rather looked into the cache. hope this answered it?
     - Yes!


### Building Apptainer images
https://coderefinery.github.io/hpc-containers/building_images/

- what would be a good/recommended way to use secrets when building a container?
    - If you need secrets during container build (for obtaining code for example), you can copy them via `%files`-block and then remove them afterwards during the installation.
    - If you need to use secrets when running the container, you can bind the files into the container when it is running. We'll talk about this later.
    - Not recommended, but if you **have** to put confidential info / secrets to containers, please use encrypted containers: https://apptainer.org/docs/user/latest/encryption.html#encryption


- what is the %post block?
    - Here you add actions that will be executed as part of the building of the image - after fetching the image. You will put all your extra software installations here. https://apptainer.org/docs/user/main/definition_files.html
        - These will be run **post** image fetching. So after the Docker image layers have been opened up.

- Can I start a blanck container, install a bunch of stuff and compile things and then lock it and open it later?
    - Yes, but this might require you to do it in your own computer because of permission issues. You can do a sandbox container where the container filesystem is blown up into a file folder: https://apptainer.org/docs/user/latest/build_a_container.html#creating-writable-sandbox-directories This is not recommended on clusters because it can create huge amount of files.
        - You could probably sidestep the filecount issue by building the container on a compute node using a ram disk, and then converting it into a single image at the end.
            - This sounds good. Because I also have project storage where the file count limit is big, or even absent.
        - But the files are in the container, so are they counted?
            - Sandbox installation creates a directory containing all the files in your container, instead of a single image.
    - Alternatively, you can create a writable overlay on top of your container where you can do modifications: https://apptainer.org/docs/user/latest/persistent_overlays.html#persistent-overlays This might also need additional permissions, so might not work in a cluster environment.

- When building a container for an HPC system do I need to have rocm or cuda on my local machine?
    - in my understanding: to build, no (since they will be downloaded and installed into the container). but you might want to have them for testing.


### MPI programs and containers
https://coderefinery.github.io/hpc-containers/mpi/

- You have said that it's important to install the same version of MPI in the container as in the system, but what is the crucial thing that actually makes the container use the resources in the system? For example, the PMI libraries in the container must use some interface to the system. Which is that? Same for interconnect: Which is the actual interface that the interconnect libraries in the container use to talk to the hardware in the system? OK, RDMA to network devices mounted into the container. So, these network devices automatically get mounted without anything having to be specified in the container definition?
    - many pieces are needed (see Figure 2 on https://coderefinery.github.io/hpc-containers/mpi/)

- I am a researcher. I am only interested in creating a container image for my code. Could you repeat what you said about generic container and optimized container. So for generic container I dont need to care about the interconnect
  - For "generic" container - look at the python example and ignore mpi stuff - if you are not using gpu/mpi things. The first part of the lesson today.

- If the staff at the HPC site basically have to create the containers to be compatible with their software and hardware, why even have containers? Why not just load the module, or help the user build that program?
    - the container can make it a bit more portable to another cluster or the same cluster after an OS upgrade. and even if portability did not get so much easier, it forced everybody to document all necessary steps and this might make manual installation easier on the next cluster.
    - You can also create pre-built images that work in most cases. For example [LAMMPS](https://hub.docker.com/r/lammps/lammps/tags) has multiple variants of prebuilt containers for different MPI flavors. These work in most cases and can provide new users easy way of accessing the software. For maximum performance, you'll want to verify that these work with the cluster's interconnect.
    - You can also share the installation with other people easier if it is stored in a single container image.

- It seems like things are going in 2 opposite directions: EasyBuild, Conda, pip, etc, try to make softwares compatible and transferable (and minimizing storage/redundancy) by keeping track of the dependencies, and in the case of Conda/virtualenv, making environments that are reproducible. Then there is containers which do the exact opposite: freeze everything in, storage is cheap! Or am I missunderstanding something in the concept?
    - I would say that the thing is more of a gradient: Easybuild/spack builds everything for your specific site, conda uses pre-built binaries that work in generic situations and containers locks the software in place. For reproducibility containers are the best: everything is there in a single file. Conda packages will disappear from the internet and for EasyBuild / Spack rebuilding is needed, which by definition will produce new binaries. But reproducibility is not everything: EasyBuild, Spack and conda are great for normal work. But if you want to make everything reproducible you can lock all of them in place by using them in a container.


### Running containers that use GPUs
https://coderefinery.github.io/hpc-containers/gpus/

- When you download these containers for Nvidia for example, do you modify the one you donwloaded, or build a new modified one with "from"? That script (accelerate_cuda.def) you run yourself?
    - the "from" image layer is kept unmodified. we create a new image with few modifications on top but down't really touch the original base image.



### Sharing reproducible containers
https://coderefinery.github.io/hpc-containers/sharing/

- Jaan: This tool help exploring container layers: https://github.com/wagoodman/dive
     - thanks!

- But even if you use the 'latest' version, the container will still be frozen, the issue is only if you wanna rebuild it?
     - exactly. rebuilding because I have lost access to the image or rebuilding to make small changes to it. but with unspecified versions I might be surprised that many other things have changed which I did not intend to change.


- About registries: Apptainer also supports [ORAS registries](https://oras.land/docs/compatible_oci_registries) for pushing images. See [apptainer registry login](https://apptainer.org/docs/user/latest/cli/apptainer_registry_login.html) on how to log in and [apptainer push](https://apptainer.org/docs/user/latest/cli/apptainer_push.html) on how to push an image

- Tip: Definition file which was used for building the image or sandbox is saved in singularity image file / sandbox (/.singularity.d/Singularity)
    - You can also use `apptainer sif dump 1 my_container.sif` to access it. See [examining container](https://coderefinery.github.io/hpc-containers/building_images/#examining-the-container) for more information.
    - nice! thanks! today I learned ...


### Concluding remarks

- Also remember to check coderefinery for upcoming courses and join our chat if you feel like participating in the project: https://coderefinery.org/
- I haven't done all the exercises in the course, so planning to do afterwards. How do I get support?
- All the exercises in the entire course, so I contact the instructors individually?
    - for the sharing episode (maybe other episodes also): you can send questions/suggestions directly to radovan.bast@uit.no
    - or also ask questions here, and Johan will send them to the correct person :) Can also be useful for others


## Please fill in the Course Evaluation Form https://www.kth.se/form/build-systems-course-evaluation

---
This is the end of the collaborative document, please write above this line*
