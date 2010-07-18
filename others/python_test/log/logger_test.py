
logfile = r'~.log'

def initlog():
    import logging

    logger = logging.getLogger()
    hdlr = logging.FileHandler(logfile)
    formatter = logging.Formatter('%(asctime)s : [%(levelname)s] %(message)s')
    hdlr.setFormatter(formatter)
    logger.addHandler(hdlr)
    logger.setLevel(logging.NOTSET)

    return logger

logger = initlog()
logger.error("error log")
logger.info("info log")
