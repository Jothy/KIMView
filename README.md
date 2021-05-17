# Template to work on. Documentation work is in progressssssssss...

![KIMView](https://github.com/Jothy/KIMView/blob/master/Icons/KIMView.png)

# KIMView: An open-source real-time tumour motion, dose, and anatomy visualisation tool for radiotherapy

## Introduction

Image-guided radiotherapy (IGRT) typically focuses on measuring geometric errors in patient’s anatomy, especially tumor position and methods to account for it. Geometric accuracy is an indirect measure of treatment quality. The metric of most interest in radiotherapy that correlates directly with clinical outcomes is dose to tumour and critical organs [[1]](https://pubmed.ncbi.nlm.nih.gov/20171515/). The goal of this project is to develop an open-source real-time tumour motion, dose, and anatomy visualisation tool for radiotherapy so the treatment team can see the dose being delivered to the dynamic patient.

## Related publications
1. [J.Selvaraj,Modelling the effect of geometric uncertainties, clonogen distribution and IMRT interpaly effect on Tumour Control Probability, 2013, PhD Thesis, University of Liverpool, UK](https://livrepository.liverpool.ac.uk/17533)


## Getting Started



### Prerequisites
KIMView depends on the following toolkits:
1. [QT 5.XX](https://www.qt.io/offline-installers) or later but not QT 6.X
2. [VTK 7.X](https://vtk.org/download/)
3. [GDCM 2.8.X](https://sourceforge.net/projects/gdcm/files/gdcm%202.x/)
4. [ITK 4.XX](https://itk.org/download/)
5. [CMake](https://cmake.org/download/)
6. [Visual Studio Community Edition](https://visualstudio.microsoft.com/vs/community/)

Either you can download the tookits and build VTK, GDCM & ITK yourself or else you can use the libs and DLLs provided.The sequence of build is VTK->GDCM->ITK. You need to use CMake to configure the build process adn genereate make fiels then build build with Visual Studio. The current version of KIMView is only compatible with the versions of libraries mentioned above.

### Installing

A step by step series of examples that tell you how to get a development env running

Say what the step will be

```
Give the example
```

And repeat

```
until finished
```

End with an example of getting some data out of the system or using it for a little demo

## Running the tests



### Break down into end to end tests

Explain what these tests test and why

```
Give an example
```

### And coding style tests

Explain what these tests test and why

```
Give an example
```

## Deployment

Add additional notes about how to deploy this on a live system

## Built With

* [QT Creator](https://www.qt.io/product/development-tools)


## Contributing

Please read [CONTRIBUTING.md](https://gist.github.com/PurpleBooth/b24679402957c63ec426) for details on our code of conduct, and the process for submitting pull requests to us.

## Versioning

We use [SemVer](http://semver.org/) for versioning. For the versions available, see the [tags on this repository](https://github.com/Jothy/KIMView/tags). 

## Authors

* **Jothy Selvaraj** - *Email: Jothybasu@gmail.com* - [Initial work](https://livrepository.liverpool.ac.uk/17533/)

See also the list of [contributors](https://github.com/your/project/contributors) who participated in this project.

## License

This project is licensed under the MIT License - see the [LICENSE.md](https://github.com/Jothy/KIMView/blob/bbe98d173ba6c38958d506e7451bf99b1e25b5a4/LICENSE) file for details

Thsi project uses Deleaker for deugging leaks.

https://www.deleaker.com/

## Acknowledgments

* Hat tip to anyone whose code was used
* Inspiration
* etc

