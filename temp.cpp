
#include <TMB.hpp>

template<class Type>
Type objective_function<Type>::operator() ()
{
    DATA_INTEGER(ntarg);    // number of targets
    DATA_VECTOR(hit);       // Obs ervations
    DATA_VECTOR(rt);        // Reaction time
    DATA_FACTOR(id);        // individuals
    DATA_FACTOR(ngroup);    // nobs per individual
    DATA_INTEGER(nsub)      // num of inidividuals
    PARAMETER_VECTOR(betas); // mu, sigma, asym
//    PARAMETER_VECTOR(log_sig_re); // random effects
//    PARAMETER_VECTOR(log_sig_re_u);
//    PARAMETER_MATRIX(sig_re_u);
    PARAMETER(log_sig1);
    PARAMETER(log_sig2);
    PARAMETER(log_sig3);
    PARAMETER(log_sig_u1);
    PARAMETER(log_sig_u2);
    PARAMETER(log_sig_u3);
    PARAMETER_VECTOR(sig_re_u1);
    PARAMETER_VECTOR(sig_re_u2);
    PARAMETER_VECTOR(sig_re_u3);
    PARAMETER_VECTOR(redund);


    Type sig1 = exp(log_sig1);
    Type sig2 = exp(log_sig2);
    Type sig3 = exp(log_sig3);

    Type sig1_u = exp(log_sig_u1);
    Type sig2_u = exp(log_sig_u2);
    Type sig3_u = exp(log_sig_u3);

    ADREPORT(sig1);
    ADREPORT(sig2);
    ADREPORT(sig3);
    ADREPORT(sig1_u);
    ADREPORT(sig2_u);
    ADREPORT(sig3_u);

     using namespace density;
     int i,j,ii; // i for id, j for observation, ii for ?

     Type g=0;
     ii = 0;
     for (i = 0; i < nsub; i++) {
         Type u1 = sig_re_u1[i+nsub];
         Type u2 = sig_re_u2[i+nsub];
         Type u3 = sig_re_u3[i+nsub];
         g -= -(log_sig_u1);
         g -= -.5*pow(u1/sig1_u, 2);
         g -= -(log_sig_u2);
         g -= -.5*pow(u2/sig2_u, 2);
         g -= -(log_sig_u3);
         g -= -.5*pow(u3/sig3_u, 2);
         vector<Type> a(3); 
         a[0] = betas[0] + u1; // theyre awfully small anyway
         a[1] = betas[1] + u2;
         a[2] = betas[2] + u3;
         
         Type tmp;
         Type f;
         // hasnt been un-oranged yet, and Im a bit lost
         for(j=0;j<ngroup(i);j++)
         {
             f = (1/ntarg)+(1-1/ntarg)*a[2]*pnorm(rt[ii],a[0],a[1]);
             tmp = (hit[ii] - f)/sig1;
             g -= -log_sig1 -log_sig2 -log_sig3 - 0.5*tmp*tmp;
             ii++;
         }

     }

return g;
}


