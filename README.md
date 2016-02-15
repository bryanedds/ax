# Ax - The Axiomatic Core C++ Library. [![Build Status](https://travis-ci.org/bryanedds/ax.svg?branch=master)](https://travis-ci.org/bryanedds/ax) [![License](https://img.shields.io/badge/license-MIT-blue.svg)](https://github.com/bryanedds/ax/blob/master/LICENSE.md) 

## Features

- A light-weight reflection system in **"reflection.hpp"**
- A serialization system in **"serialization.hpp"**
- A publisher-neutral C++ event system in **"events.hpp"**
- A dynamic property system in **"properties.hpp"**
- A generalized meta-language system in **"metalang.hpp"**
- Optimized dynamic type casting system in **"casting.hpp"**
- Functional operations map, filter, and fold (with more coming soon!) in **"functional.hpp"**
- Algebraic data types including option, either, choice, pair and record, w/ macros for automatic derivation, 
and a type-safe pattern matching system in **"algebraic.hpp"**
- A couple other odds and ends in **"utility.hpp"**

Ax was built in the clean and modular Data Abstration Style - https://github.com/bryanedds/das

This library makes a good base for non-trivial projects like renderers, game engines, and other real-ass shit that absolutely *needs* to be built with good programming semantics.

## WIP
Note this project has not reached alpha-status, so not recommended for use in production without serious care. There's even some undefined behavior lingering in the reflection system I'm trying to remove. Tests and fixes are coming along over time, however :)
