# Add a new service checklist

* according to `ttsService.h`
* implement `TextToSpeechService::speak`
* implement `TextToSpeechConfigWidget::save`
* DONE.

# Design

TL;DR: Simply do nothing, avoid almost all temptation to do 💩 abstraction 💩 unless absolutely necessary.

Goals:

* Make adding new services easy.
* Allow modifying / evolving any one of the services arbitrarily without incurring the need to touch another.

Code:

* Config file deserialization and Service state mutating won't exist in parallel.
* Service construct only based on config file. One single deterministic routine.

```
(1) Service configure GUI --write--> (2) Service's config file --create--> (3) Live Service Object
```

(1) will neither mutate nor access (3).

construct (3) only according to (2).

(1) only write to (2).

* Service construction will be done on the consumer side to avoid sharing constructor
* Service can be cast to `Service`, which only has `speak` and destructor, since the consumer should not care anything else after construction.
* Services don't share global config, don't share config mechanisms.
* Does not assume services have any similarity other than the fact they may `speak`.
* Avoid any "abstraction" across services.

Rational:

* Services are different.
* Construction materials needed by services are different.
* Services on earth are limited, thus the boilerplate caused by fewer useless abstractions is also limited
* The consumer will use services incredibly creative in the future.
* Abstracting seemingly similar but different things will eventually blunder.
