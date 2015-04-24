# Q: How do I get just one click (state toggle) from a digital device? #

When mouse/wand clicks are setup to register as digital button presses it is hard to get just one click.

# A: Use edge triggering on `vjDigital` or on `gadget::Digital` devices. #

You will want to look at ''edge triggering'' on `vjDigital` or on `gadget::Digital` devices. An edge trigger gives you 4 states: on, off, just-on, or just-off. VR Juggler digital devices do edge triggering for you automatically. You will want to test for  just-on  so that your presses do not keep retriggering. For example, you can test it in the following way to see when the button is first pressed:

```
if (vjDigital::TOGGLE_ON == digital_dev->getData())
{
   // Button just pressed
}
```

For VR Juggler 1.1 and beyond, test against `gadget::Digital::TOGGLE_ON`.

Back to [FAQ](LiveFaq.md)