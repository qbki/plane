import createEngineModule from 'plane'

import './style.css';

function getElement<T extends HTMLElement>(id: string, constructor: new (...args: any[]) => T): T {
  const element = document.getElementById(id);
  if (element === null) {
    throw new Error(`Element "${id}" was not found`);
  }
  if (!(element instanceof constructor)) {
    throw new Error(`Unexptected type of the found element`);
  }
  return element as T;
}

function getCanvasScreen() {
  return getElement('canvas-screen', HTMLCanvasElement);
}

function getPreLoadingScreen() {
  return getElement('pre-loading-screen', HTMLElement);
}

function getLogScreen() {
  return getElement('log-screen', HTMLElement);
}

const switcher = (() => {
  const canvasScreen = getCanvasScreen();
  const preLoadingScreen = getPreLoadingScreen();
  const logScreen = getLogScreen();
  const hide = (element: HTMLElement) => element.classList.add('hidden');
  const show = (element: HTMLElement) => element.classList.remove('hidden');
  const toggle = (element: HTMLElement) => {
    if (element.classList.contains('hidden')) {
      show(element);
    } else {
      hide(element);
    }
  };
  return {
    toCanvas: () => {
      hide(preLoadingScreen);
      show(canvasScreen);
    },
    toggleLog: () => {
      toggle(logScreen);
    },
  }
})();

const log = (() => {
  const logScreen = getLogScreen();
  return (text: string) => {
    logScreen.textContent += `${text}\n`;
    console.log(text);
  }
})();

export const moduleConfig: Parameters<typeof createEngineModule>[0] = {
  noExitRuntime: true,

  print: (text: string) => {
    log(text);
  },

  canvas: (() => {
    const canvasScreen = getCanvasScreen();
    // As a default initial behavior, pop up an alert when webgl context is lost. To make your
    // application robust, you may want to override this behavior before shipping!
    // See http://www.khronos.org/registry/webgl/specs/latest/1.0/#5.15.2
    canvasScreen.addEventListener('webglcontextlost', (e) => {
      alert('WebGL context lost. You will need to reload the page.');
      e.preventDefault();
    }, false);
    canvasScreen.addEventListener('contextmenu', (event) => {
      event.preventDefault()
    });
    return canvasScreen;
  })(),

  monitorRunDependencies: (() => {
    let totalDependencies = 0;
    return (left: number) => {
      totalDependencies = Math.max(totalDependencies, left);
      log(left
        ? `Preparing... (${totalDependencies - left}/${totalDependencies})`
        : 'All downloads complete.');
    };
  })(),

  preRun: [
    (module) => {
      window.onerror = (_1, _2, _3, _4, error) => {
        const isEmscriptenError = error != null
                                  && error.constructor === Number;
        if (isEmscriptenError) {
          const [errorType, errorMessage] = module.getExceptionMessage(error);
          console.error(`${errorType}: ${errorMessage}`);
        }
      };
    },
    () => {
      window.addEventListener('keydown', (event) => {
        if (event.code === 'Backquote') {
          switcher.toggleLog();
        }
      })
    }
  ]
};

createEngineModule(moduleConfig)
  .then(() => switcher.toCanvas());
