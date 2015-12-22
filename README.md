
## Free and forced reaction time datasets, plus analysis

For example, to read in the csv containing the data for the free RT...

    free_rt <- read.csv(system.file("extdata", "bigrt.csv", package = 'multimles'),
                   header = FALSE,
                   col.names = c('id', 'RT', 'reachDir', 'hit'))

and for the forced RT,

    forced_rt <- read.csv(system.file("extdata", "bigtr.csv", package = 'multimles'),
                   header = FALSE,
                   col.names = c('id', 'RT', 'reachDir', 'hit'))

To clean up the data a bit, try things like

    free_rt <- free_rt[complete.cases(free_rt),]
    free_rt <- free_rt[!(free_rt$RT > 0.5),]
    free_rt <- free_rt[!(free_rt$RT < 0),]
    # Should have 1860 obs. left over (check with str())
    
Or (in `dplyr` syntax):

    free_rt <- free_rt %>% 
                   filter(!is.nan(reachDir), RT < 0.5, RT > 0)

Alternatives are left as an exercise to the reader.

## MLE
There were eight targets in this experiment. The objective function in `MATLAB` is specified as:

        LL = @(params) -sum(hit.*log((1/8+asymptErr*normcdf(RT,params(1),params(2))*7/8)) + (1-hit).*log(1-(1/8+asymptErr*normcdf(RT,params(1),params(2))*7/8)));

Where `asymptErr` was the upper asymptote to the probit curve (starting value of 0.9), `params(1)` was the mean (start 0.3), and `params(2)` was the sd (start 0.1).

A way to do it in R is with the `bbmle` package, a wrapper around the `mle`? function plus a number of very useful helper functions.

    llsig <- function(mu, sigma, asymptErr, rt, hit, alpha, Ntargs){
        p1 <- log(1/Ntargs + (asymptErr - 1/Ntargs) * pnorm(rt, mu, sigma))
        p2 <- log(1 - (1/Ntargs + (asymptErr - 1/Ntargs) * pnorm(rt, mu, sigma)))
        -sum(hit %*% p1 + (1 - hit) %*% p2) - alpha * sigma^2
    }
    
    start_vals <- list(mu = 0.3, sigma = 0.1, asymptErr = 0.9)
    subdat <- dplyr::filter(forced_rt, id == 3)
    mod1 <- mle2(llsig, 
               start = start_vals, 
               method = "BFGS", 
               optimizer = "optim",
               data = list(rt = subdat$RT, hit = subdat$hit),
               fixed = list(alpha = 0, Ntargs = 8))

Interpretation isn't as direct, but this sort of thing can be fit with `lme4`.

    library(lme4)
    library(psyphy)
    mlme <- glmer(hit ~ RT + (1|id), 
                  data = forced_rt, 
                  family = binomial(link = mafc.probit(8)))
    lmeprof <- profile(mlme)
    df <- forced_rt
    df$pred <- predict(mlme, type = 'response')
    mlme2 <- update(mlme, .~. + (0 + RT|id), nAGQ = 1) # enforcing correlation doesn't work

