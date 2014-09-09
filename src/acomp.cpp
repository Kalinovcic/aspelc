/* 
 * Copyright (C) 2014 Lovro Kalinovcic
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 * 
 * File: acomp.cpp
 * Description: Contains the main function.
 * Author: Lovro Kalinovcic
 * 
 */

#include <cstdlib>

#include <iostream>
#include <fstream>

#include <vector>

#include "translator.h"
#include "a10/translator.h"
#include "a10/scanner.h"

#include "scanner.h"

#define ASPELC_VERSION              "aspelc v1.0"
#define SUPPORTED_STANDARDS         { "a10", "" }
#define DEFAULT_STANDARD            "a10"

struct CompilerJob
{
    std::string source;
    std::string output;

    std::string standard;

    CompilerJob(std::string source, std::string output, std::string standard)
    {
        this->source = source;
        this->output = output;

        this->standard = standard;
    }

    std::string toString()
    {
        return source + " -> " + output + " [using " + standard + "]";
    }
};

std::vector<CompilerJob> jobs;

void abort(std::string reason)
{
    std::cerr << "\nerror: fatal: " << reason << "\n";
    exit(1);
}

bool startsWith(std::string const& str, std::string const& beginning)
{
    if(str.length() >= beginning.length())
        return str.compare(0, beginning.length(), beginning) == 0;
    return false;
}

bool endsWith(std::string const& str, std::string const& ending)
{
    if(str.length() >= ending.length())
        return str.compare(str.length() - ending.length(), ending.length(), ending) == 0;
    return false;
}

std::string nextArgument(int* index, int argc, char** argv)
{
    (*index)++;
    if((*index) >= argc)
        abort("expected argument after " + std::string(argv[*index - 1]));
    return std::string(argv[*index]);
}

std::string genOutputPath(std::string sourcePath)
{
    int extensionSize = 6;
    if(!endsWith(sourcePath, ".aspel"))
    {
        std::cout << "warning: deprecated file extension for \"" << sourcePath << "\"\n";
        extensionSize = 0;
        for(int i = sourcePath.length() - 1; i >= 0; i--)
            if(sourcePath[i] == '.')
            {
                extensionSize = sourcePath.length() - i;
                break;
            }
    }

    std::string outputPath = sourcePath.substr(0, sourcePath.length() - extensionSize).append(".aml");
    if(outputPath == sourcePath)
        abort("couldn't generate output path [paths equal]");
    return outputPath;
}

void displayVersion()
{
    std::cout << ASPELC_VERSION << "\n";
}

void displayHelp()
{
    std::cout << "Usage: aspelc [<option> | <file>]+\n";
    std::cout << "Options:\n";
    std::cout << "  --help             Display this information\n";
    std::cout << "  --version          Display compiler version\n";
    std::cout << "  --std-support      Display a list of supported standards\n";
    std::cout << "  --std-default      Display the default standard\n";
    std::cout << "  -std<standard>     Assume that the input sources are for <standard>\n";
    std::cout << "                     If <standard> is 'def', the default standard will be used.\n";
    std::cout << "  -q                 Disable compiler output\n";
    std::cout << "  -o <file>          Manually set the output file for the next job to <file>\n";
    std::cout << "\n";
}

void displaySupportedStandards()
{
    std::string supportedStandards[] = SUPPORTED_STANDARDS;
    std::cout << "supported standards: ";

    if(supportedStandards[0] == "")
    {
        std::cout << "none\n";
        return;
    }

    int index = 0;
    while(true)
    {
        std::cout << supportedStandards[index];
        index++;
        if(supportedStandards[index] == "")
            break;
        std::cout << ", ";
    }
    std::cout << "\n";
}

void displayDefaultStandard()
{
    std::cout << "stddef=" << DEFAULT_STANDARD << "\n";
}

int main(int argc, char** argv)
{
    bool quiet = false;

    std::string aspelStandard = DEFAULT_STANDARD;
    std::string outputPath = "";

    if(argc == 1) abort("no command options or input files");

    for(int argi = 1; argi < argc; argi++)
    {
        std::string arg(argv[argi]);
        if(startsWith(arg, "--"))
        {
            arg = arg.substr(2);
            if(arg == "version") displayVersion();
            else if(arg == "help") displayHelp();
            else if(arg == "std-support") displaySupportedStandards();
            else if(arg == "std-default") displayDefaultStandard();
            else abort("invalid argument \"--" + arg + "\"");
        }
        else if(startsWith(arg, "-"))
        {
            arg = arg.substr(1);
            if(arg == "q") quiet = true;
            else if(arg == "o") outputPath = nextArgument(&argi, argc, argv);
            else if(startsWith(arg, "std"))
            {
                aspelStandard = arg.substr(3);
                if(aspelStandard == "") abort("invalid standard \"-" + arg + "\"");
                if(aspelStandard == "def") aspelStandard = DEFAULT_STANDARD;
            }
            else abort("invalid argument \"-" + arg + "\"");
        }
        else
        {
            std::string usedOutputPath = outputPath;
            if(outputPath == "") usedOutputPath = genOutputPath(arg);
            outputPath = "";

            CompilerJob job(arg, usedOutputPath, aspelStandard);
            jobs.push_back(job);
        }
    }

    for(unsigned int jobi = 0; jobi < jobs.size(); jobi++)
    {
        CompilerJob job = jobs[jobi];
        if(!quiet) std::cout << "job: " << job.toString() << "";

        std::ifstream in;
        std::ofstream out;

        LexicalScanner* scanner = 0;
        if(job.standard == "a10") scanner = new LexicalScannerA10(in);

        if(!scanner)
        {
            if(!quiet) std::cout << " - failed\n";
            abort("invalid standard \"" + job.standard + "\"");
        }

        Translator* translator = 0;
        if(job.standard == "a10") translator = new TranslatorA10(*scanner, out);

        if(!translator)
        {
            if(!quiet) std::cout << " - failed\n";
            abort("invalid standard \"" + job.standard + "\"");
        }

        in.open(job.source.c_str(), std::ios::in | std::ios::binary);
        out.open(job.output.c_str(), std::ios::out);
        if(!in.good())
        {
            if(!quiet) std::cout << " - failed\n";
            abort("file not found \"" + job.source + "\"");
        }

        translator->translate();

        in.close();
        out.close();

        if(!quiet) std::cout << " - done\n";
    }

	return EXIT_SUCCESS;
}
