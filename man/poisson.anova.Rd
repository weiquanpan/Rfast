\name{Analysis of variance with a count variable}
\alias{poisson.anova}
\alias{geom.anova}
\title{
Analysis of variance with a count variable
}

\description{
Analysis of variance with a count variable.
}

\usage{
poisson.anova(y, ina, logged = FALSE)
geom.anova(y, ina, type = 1, logged = FALSE)
}

\arguments{
\item{y}{
A numerical vector with discrete valued data, i.e. counts.
}
\item{ina}{
A numerical vector with discrete numbers starting from 1, i.e. 1, 2, 3, 4,... or a factor variable. 
This is suppose to be a categorical predictor. If you supply a continuous valued vector the function 
will obviously provide wrong results.
}
\item{type}{
This rgument is for the geometric distribution. Type 1 refers to the case where the minimum is zero and type 2 for the case of the minimum being 1. 
}
\item{logged}{
Should the p-values be returned (FALSE) or their logarithm (TRUE)?
}
}

\details{
This is the analysis of variance with Poisson or geometric distributed data. What we do is a log-likelihood ratio test. 
However, this is exactly the same as Poisson regression with a single predictor variable who happens to be 
categorical. Needless to say that this is faster function than the glm command in R. For the same purpose with 
a Bernoulli variable use \code{\link{g2Test}}.
}

\value{
A vector with two values, the difference in the deviances and the relevant p-value.  
}

\author{
Michail Tsagris

R implementation and documentation: Michail Tsagris <mtsagris@yahoo.gr> and Manos Papadakis <papadakm95@gmail.com>.
}
%\note{
%%  ~~further notes~~
%}

\seealso{
\code{ \link{logistic.cat1}, \link{g2Test}, \link{poisson.anovas}, \link{anova}, \link{poisson_only}, 
\link{poisson.mle}
}
}

\examples{
y <- rpois(500, 10)
ina <- as.factor( rbinom(500, 4, 0.5) ) 
a1 <- poisson.anova(y, ina) 
a2 <- glm(y ~ ina, poisson) 
a1
anova(a2, test = "Chisq")
y <- rgeom(500, 0.7)
geom.anova(y, ina) 
}

\keyword{ Analysis of variance }
\keyword{ Poisson distribution }
\keyword{ geometric distribution }