#include "frontend.h"
#include "cowboy_phrases.h"


void InitLatexFile (TreeNode* root)
{
    assert (root != nullptr);
    FILE* out_file = get_file ("data/output.tex", "w+");

    const char header[] = R"(
    \documentclass{article}

    %  Русский язык

    \usepackage[T2A]{fontenc}			% кодировка
    \usepackage[utf8]{inputenc}			% кодировка исходного текста
    \usepackage[english,russian]{babel}	% локализация и переносы
    \usepackage{unicode-math}
    \usepackage[top=3in]{geometry}

    % Рисунки
    \usepackage{graphicx, float}
    \usepackage{wrapfig}

    \usepackage{eso-pic,graphicx}   

    \usepackage{xcolor}

    \makeatletter
    \newcommand{\globalcolor}[1]{%
    \color{#1}\global\let\default@color\current@color
    }
    \makeatother

    \AtBeginDocument{\globalcolor{black}}

    \title{Wild wild west derivative counter}
    \author{Marty Bebrou Smith}
    \date{November 1897}

    \begin{document}
    \maketitle
    \AddToShipoutPictureBG{\includegraphics[width=\paperwidth,height=\paperheight]{funny_pics/back5.png}}
    )";

    _print (header);
  
    char introduction[] = R"(

        You've been slepping so long, that Calculus anigilated all humanity and whole modern civilization was vanished.
        The humanity had to start over to restore all knowledge we lost. Unfortunately, everyone decided to become stupid cowboys and
        live in the world of Wild Wild West.

        I fucking like this live, you can bang as many hot chicks as you want, despite sometimes fuckers like you come to me
        to solve this boring equations and take derivatives.

        Oh look, a nigger is running down the hill *bang* *bang*
        
        
        Ok, ok, let's calculate this bullshit.

        \begin{center}\begin{center} \includegraphics[scale=0.6]{funny_pics/cowboy.jpg} \end{center}\end{center}
        \begin{center}
        $\clubsuit$~$\clubsuit$~$\clubsuit$
        \end{center}

    )";


    _print (introduction);

    // CalcFuncInPoint (out_file, root);

    PrintGraphic (root, -50, 50, out_file);

    fclose (out_file);

    PrintBranch (root, ORIGIN);
    
}


void GeneratePdf ()
{
    FILE* out_file = get_file ("data/output.tex", "a");

    char ending_lines[] = R"(
        The solution is pretty simple and you definetely can do it \textbf{yourself}
        \end{document}
    )";

    _print (ending_lines);

    fclose (out_file);

    system ("run_latex.bat");
}


void PrintBranch (TreeNode* root, PrintTypes mode)
{
    assert (root != nullptr);

    FILE* out_file = get_file ("data/output.tex", "a");
    
    static int useless_phrases_counter = 0;

    if (mode == ORIGIN)
    {
        DrawTree (root);

        _print ("Alright fella, let's look wat we got, i haven't seen so beautiful trees for ages:\n");   
        
        _equation(PrintInOrder (root, out_file));
    }
    else if (mode == DERIVATIVE)
    {
        _print (UselessPhrases[useless_phrases_counter++]);

        _equation(PrintInOrder (root, out_file));

        _SimplifyTree (root);
    }
    else if (mode == RESULT)
    {
        SimplifyTree (root);

        _print ("Here is whach you got, fella. Now let's drink some whiskey and shoot niggers.");
        _print (R"(\begin{center} \includegraphics[scale=0.6]{funny_pics/slave.jpg} \end{center})");
        _equation (PrintInOrder (root, out_file));
    }

    _print ("\\begin{center} $\\clubsuit$~$\\clubsuit$~$\\clubsuit$ \\end{center}");
    
    fclose (out_file);
}


void CalcFuncInPoint (FILE* out_file, TreeNode* root)
{
    assert (root != nullptr);
    double val = 0;

    printf ("Where do you want to count you function fella?\n");
    scanf ("%lg", &val);

    _print ("\\textbf{Got to calculate function in point} %lg\n", val);

    double tmp =  CalcTree (root, val);
    printf ("Calculated function in point, result: %lg\n", tmp);

    _print ("The result is %lg", tmp);

    _print ("\\begin{center} $\\clubsuit$~$\\clubsuit$~$\\clubsuit$ \\end{center}");
}


void PrintInOrder (TreeNode* node, FILE* out_file)
{
    assert (node != nullptr);

    static int node_counter = 0;


    _print ("{");

    bool need_div  = isNeedDivision (node);

    bool need_frac = (node->type == OP_T && node->value.op_val == DIV);

    if (need_frac) _print ("\\frac{");

    if (node->left && !IsFictiveZero (node->value.op_val))
    {
        if (need_div) _print ("{(");
        PrintInOrder (node->left, out_file);
        if (need_div) _print (")}");
    }

    if (!need_frac)
    {
        if  (node->type == NUM_T)
        {
            _print ("%lg", node->value.dbl_val);
        }
        else if (node->type == OP_T)
        {    
            _print ("%s", GetOpSign (node->value.op_val));
        }
        else
        {
            _print ("%s", node->value.var_name);
        }
    }
    else 
    {
        _print ("}{");
    }

    if (node->right)
    {
        if (need_div) _print ("{(");
        PrintInOrder (node->right, out_file);
        if (need_div) _print (")}");
    }

    if (need_frac) _print ("}");

    _print ("}");
} 


void PrintGraphic (TreeNode* root, int begin_x, int end_x, FILE* out_file)
{
    assert (root != nullptr);
    FILE* graph_data = get_file ("data/graphic_data.txt", "w+");

    for (double x = begin_x; x <= end_x; x++)
    {
        double func_res = CalcTree (root, x);
        fprintf (graph_data, "%lg %lg\n", x, func_res);
    }

    fclose (graph_data);

    system ("src\\python_graphics\\draw_graph.py");

    _print (R"(\begin{center} \includegraphics[scale=0.6]{function_graph.png} \end{center})");
}


void PrintMacloren (TreeNode* root, int steps)
{
    assert (root != nullptr);

    FILE* out_file = get_file ("data/output.tex", "a");

    _print ("Alright fella, let's make this shit called Macloren,"
            "there will be only %d steps, cause i don't know how to count more.", steps);

    _print ("Basicly the main formula will look like that\n \\[ f(x) = ");
    for (int i = 0; i <= steps; i++)
    {
        if (i == 0) _print("f(0) + ");
        else        _print("\\frac{f^{(%d)}(0)}{%d!}\\cdot X + ", i, i);
    }
    _print ("\\text{...}\\]\n");

    TreeNode* cur_der = root;

    for (int i = 0; i <= steps; i++)
    {
        _print("\\[ f^{(%d)}(0) = ", i);

        _print ("%lg", CalcTree (cur_der, 0));

        _print("\\]");

        cur_der = GetDerivative (cur_der, false);
    }

    fclose (out_file);
}


bool IsFictiveZero (Operations op)
{
    if (op == SIN ||
        op == COS ||
        op == TG  ||
        op == CTG ||
        op == LN) return true;

    return false; 
}


bool isNeedDivision (TreeNode* op_node)
{
    Operations cur_op = op_node->value.op_val;

    if (op_node->type != OP_T) return false;

    if (cur_op == ADD ||
        cur_op == SUB) return false;

    if (cur_op == SIN ||
        cur_op == COS ||
        cur_op == LN) return true;
    
    TreeNode* left_child  = op_node->left;
    TreeNode* right_child = op_node->right;

    if (left_child->left || left_child->right ||
        right_child->left || right_child->right) return true;
    
    return false;

}


char* GetOpSign (Operations op)
{
    switch (op)
    {
    case ADD:
        return "+";

    case SUB:
        return "-";

    case DIV:
        return "/";

    case MUL:
        return "\\cdot";

    case POW:
        return "^";

    case SIN:
        return "\\sin";

    case TG:
        return "tg";

    case CTG:
        return "ctg";
    
    case COS:
        return "\\cos";
    
    case LN:
        return "ln";

    case LOG:
        return "log";

    case ARCCOS:
        return "arccos";

    case ARCSIN:
        return "ARCSIN";

    default:
        return "?";
    }
} 


#define __print(...) fprintf (dot_file, __VA_ARGS__)

void DrawTree (TreeNode* root)
{
    assert (root != nullptr);

    static int img_counter = 0;

    FILE* dot_file = get_file ("data/graph.dot", "w+");
    
    // Writing header info
    const char header[] = R"(
    digraph g {
        dpi      = 200;
        fontname = "Comic Sans MS";
        fontsize = 20;
        rankdir   =  TB;
        edge [color = darkgrey, arrowhead = onormal, arrowsize = 1, penwidth = 1.2]
        graph[fillcolor = lightgreen, ranksep = 1.3, nodesep = 0.5,
        style = "rounded, filled",color = green, penwidth = 2]

    )";

    
    __print (header);

    InitGraphvisNode (root, dot_file);

    RecursDrawConnections (root, dot_file);

    __print ("}\n");
    
    // Executing dotfile and printing an image

    printf ("Closing the dot file");
    fclose (dot_file);

    char src[MAX_SRC_LEN] = "";

    sprintf (src, "dot -Tpng data/graph.dot -o data/pretty_tree%d.png", img_counter);

    system (src);

    img_counter++;

    return;
}


void InitGraphvisNode (TreeNode* node, FILE* dot_file)   // Recursivly initialises every node 
{
    assert (node != nullptr);

    if (node->type == NUM_T)
        __print ("Node%p[shape=record, width=0.2, style=\"filled\", color=\"red\", fillcolor=\"#DB8E21\","
                "label=\" {Type: number | value: %lg}\"] \n \n",
                node, node->value.dbl_val);

    else if (node->type == OP_T)
        __print ("Node%p[shape=record, width=0.2, style=\"filled\", color=\"red\", fillcolor=\"#DBB585\","
                "label=\" {Type: operation | value: %s}\"] \n \n",
                node, GetOpSign(node->value.op_val));

    else if (node->type == VAR_T)
        __print ("Node%p[shape=record, width=0.2, style=\"filled\", color=\"red\", fillcolor=\"#19DB27\","
                "label=\" {Type: variable | value: %s}\"] \n \n",
                node, node->value);

    else
    {
        __print ("Node%d[shape=record, width=0.2, style=\"filled\", color=\"red\", fillcolor=\"red\","
                "label=\" {Op type: %d | value: unknown type}\"] \n \n",
                node, node->type);
    }

    if (node->left) InitGraphvisNode (node->left, dot_file);
    if (node->right) InitGraphvisNode (node->right, dot_file);

    return;
}


void RecursDrawConnections (TreeNode* node, FILE* dot_file)
{
    assert (node != nullptr);

    if (node->left)
    {
        __print ("Node%p->Node%p\n", node, node->left);
        RecursDrawConnections (node->left, dot_file);
    } 
    if (node->right)
    {
        __print ("Node%p->Node%p\n", node, node->right);
        RecursDrawConnections (node->right, dot_file);
    } 

    return;
}


#undef __print

