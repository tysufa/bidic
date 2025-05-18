#pragma once
#include "scug.hh"
#include "scav.hh"
#include <memory>

std::unique_ptr<scug::Program> Parse(std::unique_ptr<scav::Program> scav);