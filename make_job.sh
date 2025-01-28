#! /bin/bash

# This script is used to create a job file for the HPC cluster.
# For each input file specified in datalist.txt, a job directory is created.
# The job directory contains a job file, a runReadTree.C, a ReadTree.C, a ReadTree.h, and a submit file.
# The ROOT macro is modified to read the input file specified in datalist.txt.

# The main part of the script starts now:

# Check if the datalist.txt file exists:
if [ ! -f datalist.txt ]; then
  echo "Error: datalist.txt file does not exist!"
  exit 1
fi

# Check if the runReadTree_template.C file exists:
if [ ! -f runReadTree_template.C ]; then
  echo "Error: runReadTree_template.C file does not exist!"
  exit 1
fi

# Check if the ReadTree_template.C file exists:
if [ ! -f ReadTree_template.C ]; then
  echo "Error: ReadTree_template.C file does not exist!"
  exit 1
fi

# Check if the ReadTree_template.h file exists:
if [ ! -f ReadTree_template.h ]; then
  echo "Error: ReadTree_template.h file does not exist!"
  exit 1
fi

# Remove joblist.txt if it already exists:
if [ -f joblist.txt ]; then
  rm joblist.txt
fi

# Loop over the input files specified in datalist.txt:
while read line; do
    # Check if the input file exists:
    # if [ ! -f $line ]; then
    #   echo "Error: input file $line does not exist!"
    #   exit 1
    # fi

    # Create a job directory: use the input file name without the .root extension and time.
    jobdir=condor_job_$(basename $line .root)_$(date +%y%m%d_%H%M%S)

    # Check if the job directory already exists:
    if [ -d $jobdir ]; then
      echo "Error: job directory $jobdir already exists!"
      exit 1
    fi
    mkdir $jobdir
    cd $jobdir

    # Create a job file from the template: job_template.sh
    # Use sed to replace the input file name in the template with the actual input file name:
    # JOB_INPUT_FILE will be replaced with the actual input file name:

    sed -e "s|JOB_INPUT_FILE|$line|g" ../job_template.sh > run_root.sh
    sed -e "s|JOB_DIR|$PWD|g" run_root.sh > run_root.sh.new
    mv run_root.sh.new run_root.sh
    chmod +x run_root.sh

    # Create a ROOT macro from the template: runReadTree_template.C
    # Use sed to replace the input file name in the template with the actual input file name:
    # INPUT_FILE will be replaced with the actual input file name:

    sed -e "s|JOB_INPUT_FILE|$line|g" ../runReadTree_template.C > runReadTree.C
    sed -e "s|JOB_DIR|$PWD|g" runReadTree.C > runReadTree.C.new
    mv runReadTree.C.new runReadTree.C

    # Create a ROOT macro from the template: ReadTree_template.C
    # Use sed to replace the input file name in the template with the actual input file name:
    # JOB_DIR will be replaced with the actual job directory name:

    sed -e "s|JOB_DIR|$PWD|g" ../ReadTree_template.C > ReadTree.C

    # Create a ROOT macro from the template: ReadTree_template.h
    # Use sed to replace the input file name in the template with the actual input file name:
    # JOB_DIR will be replaced with the actual job directory name:

    sed -e "s|JOB_DIR|$PWD|g" ../ReadTree_template.h > ReadTree.h

    # Generate the corresponding submit file
    # Use sed to replace the job directory name in the template with the actual job directory name:
    # JOB_DIR will be replaced with the actual job directory name:

    sed -e "s|JOB_DIR|$PWD|g" ../job_template.sub > job_$jobdir.sub
    sed -e "s|JOB_INPUT_FILE|$line|g" job_$jobdir.sub > job_$jobdir.sub.new
    sed -e "s|JOB_EXECUTABLE|$PWD/run_root.sh|g" job_$jobdir.sub.new > job_$jobdir.sub
    rm job_$jobdir.sub.new

    # Go back to the parent directory:
    cd ..

    # Store the job directory name in the joblist.txt file:
    echo $PWD/$jobdir >> joblist.txt

done < datalist.txt

# Generate a run_all.sh script to submit all the jobs:
echo "#! /bin/bash" > run_all.sh
echo "" >> run_all.sh
echo "# This script is used to submit all the jobs to the HPC cluster." >> run_all.sh
echo "" >> run_all.sh
echo "# The main part of the script starts now:" >> run_all.sh
echo "" >> run_all.sh
echo "for dir in \$(cat joblist.txt); do" >> run_all.sh
echo "  cd \$dir" >> run_all.sh
echo "  condor_submit job_\$dir.sub" >> run_all.sh
echo "  cd .." >> run_all.sh
echo "done" >> run_all.sh

