#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

struct Process {
    int id;
    int arrivalTime;
    int burstTime;
    int remainingTime;
    int startTime;
    int finishTime;
    int waitingTime;
    int turnaroundTime;
};

bool compareArrivalTime(const Process& a, const Process& b) {
    return a.arrivalTime < b.arrivalTime;
}

bool compareBurstTime(const Process& a, const Process& b) {
    return a.burstTime < b.burstTime;
}
void FCFS(vector<Process>& processes, int contextSwitchTime) {
    int currentTime = 0;
    for (auto& p : processes) {
        if (p.arrivalTime > currentTime)
            currentTime = p.arrivalTime;
        p.startTime = currentTime;
        currentTime += p.burstTime;
        p.finishTime = currentTime;
        p.turnaroundTime = p.finishTime - p.arrivalTime;
        p.waitingTime = p.turnaroundTime - p.burstTime;
        currentTime += contextSwitchTime;
    }
}

void SRT(vector<Process>& processes, int contextSwitchTime) {
    int n = processes.size();
    int currentTime = 0;
    vector<int> remainingTime(n);
    for (int i = 0; i < n; ++i)
        remainingTime[i] = processes[i].burstTime;

    int completed = 0;
    while (completed < n) {
        int shortest = -1;
        for (int i = 0; i < n; ++i) {
            if (processes[i].arrivalTime <= currentTime && remainingTime[i] > 0) {
                if (shortest == -1 || remainingTime[i] < remainingTime[shortest]) {
                    shortest = i;
                }
            }
        }

        if (shortest == -1) {
            currentTime++;
            continue;
        }

        remainingTime[shortest]--;
        currentTime++;

        if (remainingTime[shortest] == 0) {
            completed++;
            processes[shortest].finishTime = currentTime;
            processes[shortest].turnaroundTime = processes[shortest].finishTime - processes[shortest].arrivalTime;
            processes[shortest].waitingTime = processes[shortest].turnaroundTime - processes[shortest].burstTime;
            currentTime += contextSwitchTime;
        }
    }
}

void RR(vector<Process>& processes, int quantum, int contextSwitchTime) {
    int n = processes.size();
    int currentTime = 0;
    queue<int> readyQueue;
    vector<int> remainingTime(n);
    for (int i = 0; i < n; ++i)
        remainingTime[i] = processes[i].burstTime;

    int completed = 0;
    while (completed < n) {
        for (int i = 0; i < n; ++i) {
            if (processes[i].arrivalTime <= currentTime && remainingTime[i] > 0) {
                int runTime = min(quantum, remainingTime[i]);
                remainingTime[i] -= runTime;
                currentTime += runTime;
                if (remainingTime[i] == 0) {
                    completed++;
                    processes[i].finishTime = currentTime;
                    processes[i].turnaroundTime = processes[i].finishTime - processes[i].arrivalTime;
                    processes[i].waitingTime = processes[i].turnaroundTime - processes[i].burstTime;
                    currentTime += contextSwitchTime;
                }
                else {
                    readyQueue.push(i);
                }
            }
        }
        if (!readyQueue.empty()) {
            int nextProcess = readyQueue.front();
            readyQueue.pop();
            readyQueue.push(nextProcess);
            currentTime += contextSwitchTime;
        }
        else {
            currentTime++;
        }
    }
}

void printFCFSResults(const vector<Process>& processes, float cpuUtilization) {
    cout << "FCFS Results:" << endl;
    cout << "Process\tStart Time\tFinish Time\tWaiting Time\tTurnaround Time\n";
    for (const auto& p : processes) {
        cout << p.id << "\t" << p.startTime << "\t\t" << p.finishTime << "\t\t" << p.waitingTime << "\t\t" << p.turnaroundTime << "\n";
    }
    cout << "CPU Utilization: " << cpuUtilization << endl;
}

void printSRTResults(const vector<Process>& processes, float cpuUtilization) {
    cout << "SRT Results:" << endl;
    cout << "Process\tStart Time\tFinish Time\tWaiting Time\tTurnaround Time\n";
    for (const auto& p : processes) {
        cout << p.id << "\t" << p.startTime << "\t\t" << p.finishTime << "\t\t" << p.waitingTime << "\t\t" << p.turnaroundTime << "\n";
    }
    cout << "CPU Utilization: " << cpuUtilization << endl;
}

void printRRResults(const vector<Process>& processes, float cpuUtilization) {
    cout << "RR Results:" << endl;
    cout << "Process\tStart Time\tFinish Time\tWaiting Time\tTurnaround Time\n";
    for (const auto& p : processes) {
        cout << p.id << "\t" << p.startTime << "\t\t" << p.finishTime << "\t\t" << p.waitingTime << "\t\t" << p.turnaroundTime << "\n";
    }
    cout << "CPU Utilization: " << cpuUtilization << endl;
}

int main() {
    ifstream inputFile("D:\shams.txt");
    if (!inputFile) {
        cerr << "Error opening file." << endl;
        return 1;
    }
    int contextSwitchTime, quantum;
    inputFile >> contextSwitchTime >> quantum;
    int id, arrivalTime, burstTime;
    vector<Process> processes;

    while (inputFile >> id >> arrivalTime >> burstTime) {
        processes.push_back({ id, arrivalTime, burstTime });
    }

    inputFile.close();

    sort(processes.begin(), processes.end(), compareArrivalTime);

    vector<Process> fcfsProcesses = processes;
    vector<Process> srtProcesses = processes;
    vector<Process> rrProcesses = processes;

    FCFS(fcfsProcesses, contextSwitchTime);
    SRT(srtProcesses, contextSwitchTime);
    RR(rrProcesses, quantum, contextSwitchTime);

    float totalExecutionTime = 0;
    for (const auto& p : processes) {
        totalExecutionTime += p.burstTime;
    }
    float cpuUtilization = (totalExecutionTime / (fcfsProcesses.back().finishTime + contextSwitchTime)) * 100;

    printFCFSResults(fcfsProcesses, cpuUtilization);
    printSRTResults(srtProcesses, cpuUtilization);
    printRRResults(rrProcesses, cpuUtilization);

    return 0;
}


