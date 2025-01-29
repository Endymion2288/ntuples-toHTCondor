# `ntuples-toHTCondor` 软件包

## 1. 简介
这个软件包用于批量处理ntuple文件。在实际使用中，我们常需要批量提交一些HTCondor作业来处理数量巨大的ntuple文件。这个软件包提供了一个简单的方法来实现这个目的。

## 2. 使用方法
### 2.1 预备文件
在使用这个软件包之前，需要准备以下文件：
- `datalist.txt`：这个文件包含了所有需要处理的ntuple文件的路径。每一行是一个ntuple文件的路径。例如：
```
/path/to/ntuple1.root
/path/to/ntuple2.root
...
```
- `runReadTree_template.C`：这个文件定义了一个`ROOT` macro，用于驱动macro`ReadTree`。其中，被处理的ntuple文件的路径通过占位符`JOB_INPUT_FILE`给出。例如：
```cpp
void runReadTree() {
    TChain *chain = new TChain("T");
    chain->Add("JOB_INPUT_FILE");
    ...
}
```
- `ReadTree_template.C`, `ReadTree_template.h`：这组文件定义了一个`ROOT` macro`ReadTree`，用于对单个文件进行处理。完成处理之后，得到的文件会被保存在相对应作业目录的`Candidates.root`中。为此，需要在`ReadTree.C`中用`JOB_DIR`明输出文件的路径。例如：
```cpp

void ReadTree() {
    ...
    TFile *f = new TFile("JOB_DIR/Candidates.root", "RECREATE");
    ...
}
```
- `runReadTree.sh`：这个文件定义了一个shell脚本，用于调用`ROOT` macro。例如：
```bash
root -x runReadTree.C
```
- `job_template.sub`：这个文件定义了一个HTCondor作业的提交文件。其中，被处理的ntuple文件的路径通过占位符`JOB_INPUT_FILE`给出，`runReadTree.sh`的路径通过占位符`JOB_EXECUTABLE`给出。`
- `make_jobs.sh`：这个文件定义了一个shell脚本，用于生成HTCondor作业目录。在这个脚本中，需要定义`runReadTree_template.C`的路径、`runReadTree.sh`的路径、`job_template.sub`的路径、`datalist.txt`的路径等信息。
- `run_all.sh`：这个文件定义了一个shell脚本，用于一次性提交所有HTCondor作业。在这个脚本中，需要定义`joblist.txt`的路径。

### 2.2 调用软件包，生成HTCondor作业目录
在准备好以上文件之后，可以通过以下命令来调用这个软件包：
```bash
./make_jobs.sh
```
这样以来就会生成一系列的HTCondor作业目录，每个目录对应一个ntuple文件。在每个目录中，包含了一个`.sub`文件，用于提交HTCondor作业。

同时，在当前目录下会生成一个列表文件`joblist.txt`，包含了所有生成的HTCondor作业目录的路径。

### 2.3 提交HTCondor作业
在生成HTCondor作业目录之后，可以通过以下命令来一次性提交所有HTCondor作业：
```bash
./run_all.sh
```

### 2.4 集合输出文件
在所有HTCondor作业完成之后，可以通过以下命令来集合所有输出文件：
```bash
hadd Candidates.root */Candidates.root
```

## 3. 开发者
- 王驰（Eric100911, eric100911@126.com or chi.w@cern.ch）
