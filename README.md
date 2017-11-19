# vtHyperNEAT

This is a demo of my C++ implementation of hypercube based extension of NEAT (NeuroEvolution of Augmenting Topologies) algorithm originally invented by Dr. Kenneth O. Stanley. Implementation developed as my ability and understanding of the topic allowed.

For the details about the algorithm and other implementations please visit the [official](http://eplex.cs.ucf.edu/hyperNEATpage/HyperNEAT.html) users page.

## Features:

* HyperNEAT-LEO variation. CPPN takes 4 inputs and bias node (two 2D coordinates + 1.0) and outputs 4 values: weight expression, weight value, bias expression, bias value.
* Disqualifying solutions returning NaN value for fitness.
* In-species champions.
* In-species selection odds based on fitness.
* Permanent innovation archive.
* Self-adapting mutation step for connection weights.
* Skipping solutions without connections.
* Species hint to optimise species assignments.
* Two benchmarks.

## Known requirements

* Debian based system.
* g++ with C++14 support.

Building on something else than Ubuntu with g++ and C++14 support installed will require a bit more effort but should be relatively easy as it is a pure C++ project with no dependencies besides STL. Still, no promises.

## Usage

Run `make` command in your shell to build the package. Parameters located in `settings` folder can be adjusted to achieve best (or worst) evaluation counts.

By default the application will try to construct a neural network to correctly classify examples from the Iris dataset. To run XOR benchmark, uncomment the line in `main()` located in `main.cpp`.

## Included benchmarks

* XOR function: simple nonlinear function meant for preliminary verification and debugging. Performs 10 runs and computes the average evaluations per solution.
* Iris dataset: simple classification challenge for further verification of the implementation. Dataset source can be found [here](https://archive.ics.uci.edu/ml/datasets/Iris). Output best viewed on 80 x 24 terminal display.

## Assumptions

* Fitness range is (0, 1], higher is better.
* Method descriptions are in `.h` files.

## Further development

No further public releases are planned.
