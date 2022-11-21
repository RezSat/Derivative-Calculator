#include "frontend.h"


void DumpTree (TreeNode* node)
{
    assert (node);

    printf ("Ptr[%p] : \n", node);
    
    if (node->type == NUM_T)
    {
        printf ("\t Node %s: left %p, right %p, parent %p, %d\n",
                node->value, node->left,
                node->right, node->parent, node->value);
    }
    else if (node->type == OP_T)
    {
        printf ("\t Node %s: left %p, right %p, parent %p, %d\n",
                node->value, node->left,
                node->right, node->parent, GetOpSign (node->value.op_val));
    }
    else
    {
        printf ("\t Node %s: left %p, right %p, parent %p, %d\n",
                node->value, node->left,
                node->right, node->parent, node->value);
    }

    if (node->left)  DumpTree (node->left);
    if (node->right) DumpTree (node->right);
}


void PrintInFile (TreeNode* root)
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


    \title{The great derivative}
    \author{Dodo}
    \date{November 2022}


    \begin{document}
    \maketitle
    )";

    fprintf (out_file, header);

    fprintf (out_file, "\\[");
    PrintInOrder (root, out_file);
    fprintf (out_file, "\\]");

    fprintf (out_file, "\n\n\t\\end{document}");

    fclose (out_file);

    system ("run_latex.bat");
}


void PrintInOrder (TreeNode* node, FILE* out_file)
{
    fprintf (out_file, "{");
    printf ("Type %d Val %d\n", node->type, node->value.op_val);

    bool need_div  = isNeedDivision (node);

    bool need_frac = (node->type == OP_T && node->value.op_val == DIV);

    if (need_frac) fprintf (out_file, "\\frac{");

    if (need_div) fprintf (out_file, "(");
    if (node->left)  PrintInOrder (node->left,  out_file);
    if (need_div) fprintf (out_file, ")");

    if (!need_frac)
    {
        if  (node->type == NUM_T)
            fprintf (out_file, "%lg", node->value.dbl_val);
        else if (node->type == OP_T)
            fprintf (out_file, "%s", GetOpSign (node->value.op_val));
        else
            fprintf (out_file, "%s", node->value.var_name);
    }
    else 
    {
        fprintf (out_file, "}{");
    }

    if (need_div) fprintf (out_file, "(");
    if (node->right) PrintInOrder (node->right, out_file);
    if (need_div) fprintf (out_file, ")");

    if (need_frac) fprintf (out_file, "}");

    fprintf (out_file, "}");

} 


bool isNeedDivision (TreeNode* op_node)
{
    if (op_node->type != OP_T) return false;
    if (op_node->value.op_val != MUL) return false;
    
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
    
    case COS:
        return "cos";
    
    case LN:
        return "ln";

    default:
        return "?";
    }
} 


#define _print(...) fprintf (dot_file, __VA_ARGS__)

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

    
    _print (header);

    InitGraphvisNode (root, dot_file);

    RecursDrawConnections (root, dot_file);

    _print ("}\n");
    
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
        _print ("Node%p[shape=record, width=0.2, style=\"filled\", color=\"red\", fillcolor=\"lightblue\","
                "label=\" {Type: number | value: %lg}\"] \n \n",
                node, node->value.dbl_val);

    else if (node->type == OP_T)
        _print ("Node%p[shape=record, width=0.2, style=\"filled\", color=\"red\", fillcolor=\"lightblue\","
                "label=\" {Type: operation | value: %s}\"] \n \n",
                node, GetOpSign(node->value.op_val));

    else if (node->type == VAR_T)
        _print ("Node%p[shape=record, width=0.2, style=\"filled\", color=\"red\", fillcolor=\"lightblue\","
                "label=\" {Type: variable | value: %s}\"] \n \n",
                node, node->value);

    else
    {
        _print ("Node%d[shape=record, width=0.2, style=\"filled\", color=\"red\", fillcolor=\"lightblue\","
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
        _print ("Node%p->Node%p\n", node, node->left);
        RecursDrawConnections (node->left, dot_file);
    } 
    if (node->right)
    {
        _print ("Node%p->Node%p\n", node, node->right);
        RecursDrawConnections (node->right, dot_file);
    } 

    return;
}


#undef _print
