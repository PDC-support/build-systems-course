---
marp: true
style: |
  section h1 {
    text-align: center;
    }
  .column {
    float: left;
    width: 50%;
    outline: 20px solid #FFF;
    border: 20px solid #AAF;
    background-color: #AAF;
    }
  .row:after {
    display: table;
    clear: both;
    }
  .columns {
    display: grid;
    grid-template-columns: repeat(2, minmax(0, 1fr));
    gap: 1rem;
  }
  .columns-left {
    background: none;
  }
  .columns-right {
    background: none;
  }
---

<!-- paginate: true -->

# Welcome to the Build Systems Course

## A course hosted by PDC, ENCCS, CodeRefinery, CSC, and Aalto Scientific Computing

Over Zoom, 8-11 October, 2024

https://www.pdc.kth.se/about/events/build-systems-course-and-hackathon-part-i-1.1346795

---

# Zoom, collaborative document, course material

- The course is given over Zoom (link sent over email)

- Main channel for questions, answers and discussion is the collaborative document https://notes.coderefinery.org/buildsystemscourse2024

- Schedule and links to course material: https://github.com/PDC-support/build-systems-course

---

# Where to run exercises?

- On the Dardel supercomputer.
   - For running exercises and simulations on the Dardel computer we have set up a time allocation edu24.build. This allocation will be active until 15 November.

- On another supercomputer/cluster

- On your local Linux or Mac OS computer. For Windows computers we recommend the installation of Windows Subsystem for Linux (WSL)

---

# Tuesday 8th October - Compilers, make, autotools

- 08:50 – 09:00 Soft start
- 09:00 – 09:10 Welcome and introduction
- 09:10 – 09:50 **Compilers/Linkers/Libraries** (Johan Hellsvik)
- 09:50 – 10:00 Break
- 10:00 – 10:50 **make** (Bjørn Lindi)
- 10:50 – 11:00 Break
- 11:00 – 12:00 **autotools** (Mikael Djurfeldt)

---

# Wednesday 9th October - CMake

- 08:50 – 09:00 Soft start
- 09:00 – 09:50 **From sources to executables, CMake syntax** (Yonglei Wang)
- 09:50 – 10:00 Break
- 10:00 – 10:50 **Target-based build systems with CMake** (Yonglei Wang)
- 10:50 – 11:00 Break
- 11:00 – 12:00 **Probing compilation, linking, and execution. Finding and using dependencies** (Qiang Li)

---

# Thursday 10th October - Spack and EasyBuild

- 08:50 – 09:00 Soft start
- 09:00 – 09:50 **Spack** (Peter Larsson)
- 09:50 – 10:00 Break
- 10:00 – 10:50 **Spack** (Peter Larsson)
- 10:50 – 11:00 Break
- 11:00 – 12:00 **EasyBuild** (Henric Zazzi)

---

# Friday 11th October - Containers

- 08:50 – 09:00 Soft start
- 09:00 – 09:50 **Overview and building containers** (Simo Tuomisto and Jaan Tollander de Balsch)
- 09:50 – 10:00 Break
- 10:00 – 10:50 **Containers and MPI/GPU** (Simo Tuomisto and Jaan Tollander de Balsch)
- 10:50 – 11:00 Break
- 11:00 – 11:45 **Best practices for sharing reproducible containers** (Maiken Pedersen and Radovan Bast)
- 11:45 – 12:00 Wrap up of the course & concluding remarks

---

# Concluding remarks

- Thank you for attending the course!

- The collaborative notes for first three days are uploaded to the course git repository. For follow-up questions on the topics of the course, please interact in the collaborative document that we will keep open till Wednesday next week

- Support channels at HPC centers in Nordic countries
  - Sweden: NAISS https://supr.naiss.se/support/
  - Norway: Sigma2 https://www.sigma2.no/user-support
  - Finland:
    - CSC https://docs.csc.fi/support/contact/
    - Aalto Scientific Computing https://scicomp.aalto.fi
    - LUMI Supercomputer https://lumi-supercomputer.eu/user-support/need-help/