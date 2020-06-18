#include <iostream>
#include <QFileInfo>
#include <QLibrary>
#include "../../include/IBrocker.h"
#include "../../include/ISolver.h"
#include "../../include/IProblem.h"
#include "../../include/ICompact.h"
#include "../../include/IVector.h"
#include "../../include/ILogger.h"

using namespace std;
typedef IBrocker *(* func)();

void help() {
    cout << endl;
    cout << "load problem - 0;" << endl;
    cout << "load solver - 1;" << endl;
    cout << "set parametes - 2;" << endl;
    cout << "set compact - 3" << endl;
    cout << "solve - 4" << endl;
    cout << "exit - 5" << endl;
}

void printVector(IVector* vec) {
    if (!vec){
        cout << "vs is nullptr" << endl;
    }
    else{
        std::cout << "[";
        for (size_t i = 0, size = vec->getDim(); i < size; i++) {
            std::cout << vec->getCoord(i);
            if (i+1 == size){
                cout << "]";
            }
            else{
                cout << ", ";
            }
        }
    }
}

RESULT_CODE loadDll(IBrocker*& brocker, IBrocker::Type type, ILogger* logger){
    cout << "Input dll:  " << endl;
    string path;
    cin >> path;

    QFileInfo fileInfo(path.c_str());
    if (!fileInfo.exists()) {
        if (logger){
         logger->log("dll was not found", RESULT_CODE::NOT_FOUND);
        }
        return RESULT_CODE::NOT_FOUND;
    }

    QLibrary lib(fileInfo.absoluteFilePath());
    if (!lib.load()) {
        if (logger){
            logger->log("dll was not load", RESULT_CODE::FILE_ERROR);
        }
        return RESULT_CODE::FILE_ERROR;
    }

    func get_brocker = reinterpret_cast<func>(lib.resolve("getBrocker"));
    if (!get_brocker) {
        if (logger){
           logger->log("cannot resolve getBrocker in dll", RESULT_CODE::NOT_FOUND);
        }

        return RESULT_CODE::NOT_FOUND;
    }

    brocker = reinterpret_cast<IBrocker*>(get_brocker());
    if (!brocker) {
        if (logger){
               logger->log("Cannot get IBrocker from dll", RESULT_CODE::NOT_FOUND);
        }
        return RESULT_CODE::NOT_FOUND;
    }

    if (brocker->getType() != type) {
        if (logger){
            logger->log("Incorrect dll type", RESULT_CODE::FILE_ERROR);
        }
        return RESULT_CODE::FILE_ERROR;
    }

    return RESULT_CODE::SUCCESS;
}

RESULT_CODE setParams(IProblem* problem, ISolver* solver, ILogger* logger) {
    if (!problem){
        if (logger){
            logger->log("In setParams problem is nullptr", RESULT_CODE::BAD_REFERENCE);
        }
        return RESULT_CODE::BAD_REFERENCE;
    }
    if (!solver){
        if (logger){
            logger->log("In setParams solver nullptr", RESULT_CODE::BAD_REFERENCE);
        }
        return RESULT_CODE::BAD_REFERENCE;
    }

    size_t solver_param_dim = solver->getParamsDim();

    cout << "input " << solver_param_dim << " solver parameters " <<endl;
    double* data = new double[solver_param_dim];

    if (!data) {
        if (logger){
            logger->log("Cannot create array", RESULT_CODE::OUT_OF_MEMORY);
        }
        return RESULT_CODE::OUT_OF_MEMORY;
    }
    for (size_t i = 0; i < solver_param_dim; i++) {
        cin >> data[i];
    }

    IVector* params = IVector::createVector(solver_param_dim, data, logger);
    if (params == nullptr) {
        if (logger){
            logger->log("In loadParams memory error", RESULT_CODE::OUT_OF_MEMORY);
        }
        delete[] data;
        return RESULT_CODE::OUT_OF_MEMORY;
    }

    RESULT_CODE rc = solver->setParams(params);
    if (rc != RESULT_CODE::SUCCESS) {
        if (logger){
            logger->log("In loadParams memory error", RESULT_CODE::OUT_OF_MEMORY);
        }
        delete[] data;
        delete params;
        return RESULT_CODE::OUT_OF_MEMORY;
    }

    delete[] data;
    delete params;
    params = nullptr;

    size_t problem_param_dim = problem->getParamsDim();
    cout << "input " << problem_param_dim << " problem parameters " <<endl;

    data = new double[problem_param_dim];
    if (!data) {
        if (logger){
            logger->log("In loadParams memory error", RESULT_CODE::OUT_OF_MEMORY);
        }
        return RESULT_CODE::OUT_OF_MEMORY;
    }
    for (size_t i = 0; i < problem_param_dim; i++) {
        cin >> data[i];
    }


    params = IVector::createVector(problem_param_dim, data, logger);
    if (!params) {
        if (logger){
               logger->log("In loadParams memory error", RESULT_CODE::OUT_OF_MEMORY);
        }
        delete [] data;
        return RESULT_CODE::OUT_OF_MEMORY;
    }
    rc = solver->setProblemParams(params);
    if (rc != RESULT_CODE::SUCCESS) {
        if (logger){
            logger->log("In loadParams memory error", RESULT_CODE::OUT_OF_MEMORY);
        }
        delete [] data;
        delete params;
        params = nullptr;
        return RESULT_CODE::OUT_OF_MEMORY;
    }

    delete [] data;
    delete params;
    params = nullptr;

    return RESULT_CODE::SUCCESS;
}

RESULT_CODE setCompact(IProblem* problem, ISolver *solver, ILogger *logger){
    if (!problem){
        if (logger){
            logger->log("In setCompact problem is nullptr", RESULT_CODE::BAD_REFERENCE);
        }
        return RESULT_CODE::BAD_REFERENCE;
    }
    if (!solver){
        if (logger){
            logger->log("In setCompact solver nullptr", RESULT_CODE::BAD_REFERENCE);
        }
        return RESULT_CODE::BAD_REFERENCE;
    }
    size_t comp_dim = problem->getArgsDim();

    cout << "Set begin compact point with " << comp_dim << " value" << endl;

    double *data = new double[comp_dim];
    if (data == nullptr) {
        if (logger){
            logger->log("In setCompact memory error", RESULT_CODE::OUT_OF_MEMORY);
        }
        return RESULT_CODE::OUT_OF_MEMORY;
    }

    for (size_t i = 0; i < comp_dim; i++) {
        cin >> data[i];
    }

    IVector* beg = IVector::createVector(comp_dim, data, logger);
    if (!beg) {
        if (logger){
            logger->log("In setCompact memory error", RESULT_CODE::OUT_OF_MEMORY);
        }
        delete[] data;
        return RESULT_CODE::OUT_OF_MEMORY;
    }

    cout << "Set end compact point with " << comp_dim << " value" << endl;
    for (size_t i = 0; i < comp_dim; i++) {
        cin >> data[i];
    }

    IVector* end = IVector::createVector(comp_dim, data, logger);
    if (end == nullptr) {
        if (logger){
            logger->log("In setCompact memory error", RESULT_CODE::OUT_OF_MEMORY);
        }
        delete[] data;
        delete beg;
        beg = nullptr;

        return RESULT_CODE::OUT_OF_MEMORY;
    }
    delete [] data;

    ICompact* compact = ICompact::createCompact(beg, end, logger);
    if (!compact) {
        if (logger){
            logger->log("In createCompact can not create compact", RESULT_CODE::OUT_OF_MEMORY);
        }
        delete beg;
        delete end;
        beg = nullptr;
        end = nullptr;

        return RESULT_CODE::OUT_OF_MEMORY;
    }

    delete beg;
    delete end;
    beg = nullptr;
    end = nullptr;

    RESULT_CODE rc = solver->setCompact(compact);
    if (rc != RESULT_CODE::SUCCESS) {
        if (logger){
            logger->log("In createCompact can not set compact", rc);
        }
        delete compact;
        compact = nullptr;

        return rc;
    }

    delete compact;
    return RESULT_CODE::SUCCESS;
}

int main(int argc, char *argv[]){
    ILogger* logger = ILogger::createLogger(nullptr);
    IBrocker *problemBrocker = nullptr;
    IBrocker *solverBrocker = nullptr;
    ISolver* solver = nullptr;
    IProblem* problem = nullptr;
    IVector* solution = nullptr;
    RESULT_CODE rc_op = RESULT_CODE::SUCCESS;
    bool set_param = false;
    bool set_compact = false;

    int command = -1;
    for(;;){
        help();
        cin >> command;

        if (command == 0){
            if (problem){
                problemBrocker->release();
                problem = nullptr;
                problemBrocker = nullptr;
            }
            if (solver){
                solverBrocker->release();
                solver = nullptr;
                solverBrocker = nullptr;
            }
            set_param = false;
            set_compact = false;
            cout << "input path to problem dll";
            rc_op = loadDll(problemBrocker, IBrocker::Type::PROBLEM, logger);
            if (rc_op != RESULT_CODE::SUCCESS) {
                if (logger){
                       logger->log("Cannot load problem dll", RESULT_CODE::NOT_FOUND);
                }
                logger->destroyLogger(nullptr);
                cout << "Cannot load problem dll";
                continue;
            }

            problem = reinterpret_cast<IProblem*>(problemBrocker->getInterfaceImpl(IBrocker::Type::PROBLEM));
            if (problem == nullptr) {
                if (logger){
                    logger->log("Cannot cast problem", RESULT_CODE::NOT_FOUND);
                }
                logger->destroyLogger(nullptr);
                problemBrocker->release();
                problemBrocker = nullptr;
                cout << "Cannot cast problem";
                continue;
            }
            cout << endl << "done" << endl;
        }
        else if (command == 1){
            set_param = false;
            set_compact = false;
            if (solution){
                delete solution;
                solution = nullptr;
            }
            if (!problem){
                cout << "first set problem (0)";
                continue;
            }
            if (solver){
                solverBrocker->release();
                solver = nullptr;
                solverBrocker = nullptr;
            }

            cout << "input path to solver dll";
            rc_op = loadDll(solverBrocker, IBrocker::Type::SOLVER ,logger);
            if (rc_op != RESULT_CODE::SUCCESS) {
                if (logger){
                       logger->log("Can not load solver dll", RESULT_CODE::NOT_FOUND);
                }
                solverBrocker->release();
                solverBrocker = nullptr;
                continue;
            }
            solver = reinterpret_cast<ISolver*>(solverBrocker->getInterfaceImpl(IBrocker::Type::SOLVER));
            if (problem == nullptr) {
                if (logger){
                    logger->log("Can not cast solver", RESULT_CODE::NOT_FOUND);
                }
                solverBrocker->release();
                solverBrocker = nullptr;
                continue;
           }
           rc_op = solver->setProblem(problem);
           if (rc_op != RESULT_CODE::SUCCESS){
               if (logger){
                   logger->log("Could not set problem to solver", rc_op);
               }
               solverBrocker->release();
               solverBrocker = nullptr;
               continue;
           }

           cout << endl << "done" << endl;
        }
        else if (command == 2){
            if (solution){
                delete solution;
                solution = nullptr;
            }
            set_param = false;
            set_compact = false;
            if(!problem){
                cout << "first set problem (0)";
                continue;
            }
            if (!solver){
                cout << "first set solver (1)";
                continue;
            }
            rc_op = setParams(problem, solver, logger);
            if (rc_op != RESULT_CODE::SUCCESS){
                solverBrocker->release();
                problemBrocker->release();
                solver = nullptr;
                problem = nullptr;
                solverBrocker = nullptr;
                problemBrocker = nullptr;
                if (logger){
                    logger->log("error while loading parameters", rc_op);
                }
                cout << "error while loading parameters, try again" << endl;
                continue;
            }
            set_param = true;

            cout << endl << "done" << endl;
        }
        else if (command == 3){
            if (solution){
                delete solution;
                solution = nullptr;
            }
            set_compact = false;
            if(!problem){
                cout << "first set problem (0)";
                continue;
            }
            if (!solver){
                cout << "first set solver (1)";
                continue;
            }
            if (!set_param){
                cout << "first set parameters (2)";
                continue;
            }
            rc_op = setCompact(problem, solver,logger);
            if (rc_op != RESULT_CODE::SUCCESS){
                solverBrocker->release();
                problemBrocker->release();
                solver = nullptr;
                problem = nullptr;
                solverBrocker = nullptr;
                problemBrocker = nullptr;
                set_param = false;
                continue;
            }
            set_compact = true;
            cout << endl << "done" << endl;
        }
        else if (command == 4){
            if(!problem){
                cout << "first set problem (0)";
                continue;
            }
            if (!solver){
                cout << "first set solver (1)";
                continue;
            }
            if (!set_param){
                cout << "first set parameters (2)";
                continue;
            }
            if (!set_compact){
                cout << "first set parameters (3)";
                continue;
            }
            if (solution){
                delete solution;
                solution = nullptr;
            }
            rc_op = solver->solve();
            if (rc_op == RESULT_CODE::SUCCESS){
                rc_op = solver->getSolution(solution);
                if (rc_op != RESULT_CODE::SUCCESS || !solution){
                    if (logger){
                        logger->log("error in building solution", rc_op);
                    }
                    solverBrocker->release();
                    problemBrocker->release();
                    solver = nullptr;
                    problem = nullptr;
                    solverBrocker = nullptr;
                    problemBrocker = nullptr;
                    set_param = false;
                    delete solution;
                    solution = nullptr;
                    set_param = false;
                    set_compact = false;
                    continue;
                }
            }
            else {
                solverBrocker->release();
                problemBrocker->release();
                solver = nullptr;
                problem = nullptr;
                solverBrocker = nullptr;
                problemBrocker = nullptr;
                set_param = false;
                set_param = false;
                set_compact = false;
                continue;
            }
            cout << "solution is : ";
            printVector(solution);
            cout << endl;

            double min_val = 0.0;
            rc_op = problem->goalFunctionByArgs(solution, min_val);
            if (rc_op != RESULT_CODE::SUCCESS){
                if (logger){
                    logger->log("Error while calculating minimum", rc_op);
                }
                solverBrocker->release();
                problemBrocker->release();
                solver = nullptr;
                problem = nullptr;
                solverBrocker = nullptr;
                problemBrocker = nullptr;
                set_param = false;
                delete solution;
                solution = nullptr;
                set_param = false;
                set_compact = false;
                continue;
            }
            cout << "minimum is: " << min_val << endl;
            cout << endl << "done" << endl;
        }
        else if (command == 5){
            if (solverBrocker){
                solverBrocker->release();
                solverBrocker = nullptr;
            }
            if (problemBrocker){
                problemBrocker->release();
                problemBrocker = nullptr;
            }
            solver = nullptr;
            problem = nullptr;
            if (solution){
                delete solution;
                solution = nullptr;
            }
            set_param = false;
            set_param = false;
            set_compact = false;
            break;
        }
    }
    return 0;
}
