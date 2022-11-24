#include "frontend.h"
#include "cowboy_phrases.h"


void InitLatexFile (TreeNode* root)
{
    FILE* out_file = get_file ("data/output.tex", "w+");

    const char header[] = R"(
    \documentclass{article}

    %  Русский язык

    \usepackage[T2A]{fontenc}			% кодировка
    \usepackage[utf8]{inputenc}			% кодировка исходного текста
    \usepackage[english,russian]{babel}	% локализация и переносы
    \usepackage{unicode-math}

    % Рисунки
    \usepackage{graphicx, float}
    \usepackage{wrapfig}


    \title{Wild wild west derivative counter}
    \author{Dodo}
    \date{November 2022}


    \begin{document}
    \maketitle
    )";

    _print (header);
  
    char introduction[] = R"(
        Welcome to derivative calculator fella, let's have a look at ya. God, what da hell is dis shit, fella?
        Ok, ok, let's calculate this bullshit.

        \begin{center}
        $\clubsuit$~$\clubsuit$~$\clubsuit$
        \end{center}
    )";

    _print (introduction);
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
    FILE* out_file = get_file ("data/output.tex", "a");
    
    static int useless_phrases_counter = 0;
    static int simplification_phrases_counter = 0;


    if (mode == ORIGIN)
    {
        _print ("Alright fella, let's look wat we got:\n");   
        
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

        _equation (PrintInOrder (root, out_file));
    }

    _print ("\\begin{center} $\\clubsuit$~$\\clubsuit$~$\\clubsuit$ \\end{center}");
    
    fclose (out_file);
}


void PrintInOrder (TreeNode* node, FILE* out_file)
{
    _print ("{");

    bool need_div  = isNeedDivision (node);

    bool need_frac = (node->type == OP_T && node->value.op_val == DIV);

    if (need_frac) _print ("\\frac{");

    if (node->left && !IsFictiveZero (node->value.op_val))
    {
        if (need_div) _print ("(");
        PrintInOrder (node->left, out_file);
        if (need_div) _print (")");
    }

    if (!need_frac)
    {
        if  (node->type == NUM_T)
            _print ("%lg", node->value.dbl_val);
        else if (node->type == OP_T)
            _print ("%s", GetOpSign (node->value.op_val));
        else
            _print ("%s", node->value.var_name);
    }
    else 
    {
        _print ("}{");
    }

    if (node->right)
    {
        if (need_div) _print ("(");
        PrintInOrder (node->right, out_file);
        if (need_div) _print (")");
    }

    if (need_frac) _print ("}");

    _print ("}");

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
    
    case SQR:
        return "№";

    case POW:
        return "^";

    case SIN:
        return "sin";

    case TG:
        return "tg";

    case CTG:
        return "ctg";
    
    case COS:
        return "cos";
    
    case LN:
        return "ln";

    default:
        return "?";
    }
} 


#define __print(...) fprintf (dot_file, __VA_ARGS__)

void DrawTree (TreeNode* root)
{
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

    fclose (dot_file);

    char src[MAX_SRC_LEN] = "";

    sprintf (src, "dot -Tpng data/graph.dot -o data/pretty_tree%d.png", img_counter);

    system (src);

    img_counter++;

    return;
}


void InitGraphvisNode (TreeNode* node, FILE* dot_file)   // Recursivly initialises every node 
{
    if (node->type == NUM_T)
        __print ("Node%p[shape=record, width=0.2, style=\"filled\", color=\"red\", fillcolor=\"lightblue\","
                "label=\" {Type: number | value: %lg}\"] \n \n",
                node, node->value.dbl_val);

    else if (node->type == OP_T)
        __print ("Node%p[shape=record, width=0.2, style=\"filled\", color=\"red\", fillcolor=\"lightblue\","
                "label=\" {Type: operation | value: %s}\"] \n \n",
                node, GetOpSign(node->value.op_val));

    else if (node->type == VAR_T)
        __print ("Node%p[shape=record, width=0.2, style=\"filled\", color=\"red\", fillcolor=\"lightblue\","
                "label=\" {Type: variable | value: %s}\"] \n \n",
                node, node->value);

    else
    {
        __print ("Node%d[shape=record, width=0.2, style=\"filled\", color=\"red\", fillcolor=\"lightblue\","
                "label=\" {Op type: %d | value: unknown type}\"] \n \n",
                node, node->type);
    }

    if (node->left) InitGraphvisNode (node->left, dot_file);
    if (node->right) InitGraphvisNode (node->right, dot_file);

    return;
}


void RecursDrawConnections (TreeNode* node, FILE* dot_file)
{
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

