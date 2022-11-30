// #include "TXLib.h"
#include "backend.h"
#include "frontend.h"

int main()
{
    Token token_array[MAX_TOKENS];

    FillTokensArray (token_array);

    PrintTokens (token_array);


    // TreeNode* root = GetGrammar();

    // InitLatexFile (root);
    
    // TreeNode* d_root = GetDerivative (root, TEX_OUT);
    // PrintBranch (d_root, RESULT);

    // PrintMacloren (root, 3);
    
    // GeneratePdf ();


    // DestructTree (root);
}